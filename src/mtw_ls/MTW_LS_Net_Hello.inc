// SPDX-License-Identifier: Apache-2.0
/* HELLO body at a validated RX command boundary. */
{
    const uint8_t c = rxHeader;
    const uint8_t s = rxStatus;

    const bool request = c == HELLO;
    /* A reset action is acted on in a request, echoed as acceptance in a
       reply. It never erases historical PEER_SEEN or pretends the MCU rebooted. */
    if (request && (s & RESET)) {
        if (phase == TX_ACK && cursor < (&nls_shared_count ? nls_shared_count : 0u)) {
            nls_shared_dirty[cursor >> 3] |= nls_shared_bit_masks[cursor & 7u];
            flags |= HAS_DIRTY;
        }
        phase = IDLE;
        cursor = 0;
        rxStage = HEADER;
        helloWanted &= (uint8_t)~RESET; // Both ends have now abandoned the old exchange.
    }
    if (s & RUN)
        flags |= PEER_SEEN;
    flags |= (ONLINE | HELLO_DONE);
    if (!(flags & BOOT_DONE)) {
        flags |= BOOT_DONE;
#if NLS_POLICY == NLS_BROADCAST
        markAll();
#elif NLS_POLICY == NLS_RECOVER
        helloWanted |= FULL;
#elif NLS_POLICY == NLS_BROADCAST_RECOVER
        if (s & SEEN)
            helloWanted |= FULL;
        else
            markAll();
#else
        (void)s;
#endif
    }
    if (request) {
        if (s & FULL)
            markAll();
        helloFrame(HELLO_REPLY, s & (FULL | RESET));
        if (phase == HELLO_WAIT)
            phase = IDLE;
    } else {
        helloWanted &= (uint8_t)~(s & (FULL | RESET));
        if (phase == HELLO_WAIT && !helloWanted)
            phase = IDLE;
    }
    /* Unrelated keepalives must not postpone a missing record ACK. */
    if (phase == IDLE) {
#if TLS_USE_IDLE == TLS_ENABLE
        deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
        deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
    }
}
