// SPDX-License-Identifier: Apache-2.0
/*
 Advance one outgoing operation directly in the NET phase. A completed timeout
 action ends this phase block, not the enclosing scheduler tick.
*/
do {
// A successful ACK can move directly to TX_NEXT in the RX fragment. That state
// immediately enqueues the next record or FINISH and arms the next wait.
// Do not expire the old ACK deadline between those two operations.
// Other expired operations restore outstanding dirty and use RESET/HELLO;
// no record is blindly replayed into an uncertain parser state.
#if TLS_USE_IDLE == TLS_ENABLE
    if (phase != TX_NEXT && (int16_t)(tinyls::kernel::idle_prev_ms - deadline16) >= 0)
#else
    if (phase != TX_NEXT && (int16_t)((uint16_t)tinyls::now - deadline16) >= 0)
#endif
    {
        if (phase == BACKOFF) {
            phase = IDLE;
#if TLS_USE_IDLE == TLS_ENABLE
            deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
            deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
        } else if (phase == IDLE || phase == HELLO_WAIT || phase == RESYNC) {
            if (phase == RESYNC)
                helloWanted |= RESET;
            if (phase == HELLO_WAIT) {
                flags &= (uint8_t)~ONLINE;
                helloWanted |= RESET;
            }
            sendHello();
            break;
        } else {
            fault(true);
            break;
        }
    }
    if (phase == IDLE) {
        if (helloWanted) {
            sendHello();
            break;
        }
        if ((flags & (HAS_DIRTY | ONLINE | HELLO_DONE)) == (HAS_DIRTY | ONLINE | HELLO_DONE)) {
            if (nextDirty(0) != 255) {
                cursor = 0;
                NLS_SERIAL.write(ASK);
#if TLS_USE_IDLE == TLS_ENABLE
                tinyls::kernel::state_mask |= tinyls::kernel::STATE_SKIP_DELTA;
#endif
                NLS_SERIAL.write((uint8_t)~ASK);
                phase = OPEN_WAIT;
#if TLS_USE_IDLE == TLS_ENABLE
                deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
                deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
            } else
                refreshDirty();
        }
    } else if (phase == TX_NEXT) {
        // Keep the per-record dirty scan here; opening/reverse scans share nextDirty().
        const uint8_t start = cursor;

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

        const uint8_t i = answer;
        if (i == 255) {
            refreshDirty();
            NLS_SERIAL.write(FINISH);
#if TLS_USE_IDLE == TLS_ENABLE
            tinyls::kernel::state_mask |= tinyls::kernel::STATE_SKIP_DELTA;
#endif
            NLS_SERIAL.write((uint8_t)~FINISH);
            phase = FINISH_WAIT;
#if TLS_USE_IDLE == TLS_ENABLE
            deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
            deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
        } else {
            cursor = i;
            sendRecord(true);
        }
    }
} while (false);
