// SPDX-License-Identifier: Apache-2.0
/*
 Logical half duplex on the native full-duplex UART wiring. One ASK opens a
 forward table pass. Each record is ID/~ID/VALUE/SUM8(VALUE); FINISH=00/FF.
 At FINISH an ASK reply both acknowledges closing and takes the reverse turn.
 Plain OK closes to idle. No SIDE, sequence byte, size-specific ASK or TX copy.

 One 16-bit deadline is reused by mutually exclusive states: idle liveness,
 operation waiting and collision backoff. RETRY does not renew an unaccepted
 record's deadline. Deadline comparisons use the cached TLS timestamp;
 only initialization samples micros(). RX/TX and protocol work
 are cooperative; UART byte IRQs are core owned. No call waits for UART completion or waits for a
 protocol response.
*/
// Mark complete bitmap bytes at once, preserving unused tail bits.
static void markAll()
{
    const uint8_t count = (&nls_shared_count ? nls_shared_count : 0u);
    if (!count)
        return;
    uint8_t *p = nls_shared_dirty;
    uint8_t fullBytes = count >> 3;
    while (fullBytes--) {
        *p++ = 255u;
    }
    const uint8_t tail = count & 7u;
    if (tail)
        *p |= nls_shared_tail_mask;
    flags |= HAS_DIRTY;
}
/* Return at/after start, never wrap within a turn. This makes a turn finite
   even if user callbacks continuously recommit already-passed variables. */
static uint8_t nextDirty(uint8_t start)
{
    uint8_t answer = 255;
    if ((flags & HAS_DIRTY) && start < (&nls_shared_count ? nls_shared_count : 0u)) {
        uint8_t b = start >> 3;
        uint8_t bits = nls_shared_dirty[b] & nls_shared_search_mask & (uint8_t)~(nls_shared_bit_masks[start & 7u] - 1u);
        for (;;) {
            if (bits) {
                uint8_t bit = 0;
                if (!(bits & 1u)) {
                    const uint8_t low = bits & 15u;
                    bit = low ? nls_shared_first_bit[low] : (uint8_t)(nls_shared_first_bit[bits >> 4] + 4u);
                }
                const uint8_t i = (uint8_t)(b * 8u + bit);
                if (i < (&nls_shared_count ? nls_shared_count : 0u))
                    answer = i;
                break;
            }
            if (++b >= (uint8_t)(((&nls_shared_count ? nls_shared_count : 0u) + 7u) / 8u))
                break;
            bits = nls_shared_dirty[b] & nls_shared_search_mask;
        }
    }
    return answer;
}
/* Only end-of-pass work checks the whole bitmap. A positive HAS_DIRTY may
   conservatively remain set while a record is awaiting ACK; ACK itself must
   not clear a commit made during that wait. */
static void refreshDirty()
{
    if (!(flags & HAS_DIRTY))
        return;
    for (uint8_t b = 0; b < (uint8_t)(((&nls_shared_count ? nls_shared_count : 0u) + 7u) / 8u); ++b)
        if (nls_shared_dirty[b])
            return;
    flags &= (uint8_t)~HAS_DIRTY;
}
// HELLO frames share their larger builder across requests and replies.
static void helloFrame(uint8_t c, uint8_t actions)
{
    const uint8_t s = ((flags & HELLO_DONE) ? RUN : 0) | ((flags & PEER_SEEN) ? SEEN : 0) | actions;
    NLS_SERIAL.write(c);
#if TLS_USE_IDLE == TLS_ENABLE
    tinyls::kernel::state_mask |= tinyls::kernel::STATE_SKIP_DELTA;
#endif
    NLS_SERIAL.write((uint8_t)~c);
    NLS_SERIAL.write(s);
    NLS_SERIAL.write((uint8_t)~s);
}
static void sendHello()
{
    helloFrame(HELLO, helloWanted);
    phase = HELLO_WAIT;
#if TLS_USE_IDLE == TLS_ENABLE
    deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
    deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
}
/* A TX record has already had nls_shared_dirty cleared at enqueue time. If its ACK is
   lost, restoring this bit is sufficient: the next pass reads current VALUE.
   Clearing a later commit here would lose an application update. */
static void fault(bool timedOut)
{
    if (phase == TX_ACK && cursor < (&nls_shared_count ? nls_shared_count : 0u)) {
        nls_shared_dirty[cursor >> 3] |= nls_shared_bit_masks[cursor & 7u];
        flags |= HAS_DIRTY;
    }
    flags &= (uint8_t)~ONLINE;
    cursor = 0;
    rxStage = HEADER;
    helloWanted |= RESET;
    if (timedOut)
        sendHello();
    else
        phase = RESYNC; // Drain until existing counter expires; no marker hunt in VALUE.
}
/*
 Read current application storage directly into native TX. The application
 does not run another main-context callback within this short scalar loop.
 There is no retained TX snapshot. On RETRY the same cursor reads the current
 value again; a commit made during the previous wait is consumed by this send.
 The native port is exclusively owned by NET and has a ring >=64 bytes.
*/
static void sendRecord(bool newDeadline)
{
    if (cursor >= (&nls_shared_count ? nls_shared_count : 0u)) {
        fault(false);
        return;
    }
    Object &o = nls_shared_objects[cursor];
    const uint8_t id = (uint8_t)(cursor + 1u);
    uint8_t sum = 0;
    NLS_SERIAL.write(id);
#if TLS_USE_IDLE == TLS_ENABLE
    tinyls::kernel::state_mask |= tinyls::kernel::STATE_SKIP_DELTA;
#endif
    NLS_SERIAL.write((uint8_t)~id);
    const uint8_t *p = o.ptr;
    uint8_t remaining = o.size;
    do {
        const uint8_t b = *p++;
        sum = (uint8_t)(sum + b);
        NLS_SERIAL.write(b);
    } while (--remaining);
    NLS_SERIAL.write(sum);
    nls_shared_dirty[cursor >> 3] &= (uint8_t)~(1u << (cursor & 7u));
    phase = TX_ACK;
    if (newDeadline) {
#if TLS_USE_IDLE == TLS_ENABLE
        deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
        deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
    }
}
