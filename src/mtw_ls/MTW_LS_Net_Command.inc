// SPDX-License-Identifier: Apache-2.0
/* Short-command body at a validated RX command boundary.
   RX already marked I/O activity before this body; replies need no second
   write to the TLS IDLE-skip bit. */
{
    const uint8_t c = rxHeader;

    switch (c) {
    case ASK:
        if (!(flags & HELLO_DONE))
            break;
        if (phase == IDLE || phase == BACKOFF) {
            NLS_SERIAL.write(OK);
            NLS_SERIAL.write((uint8_t)~OK);
            phase = RX_TURN;
#if TLS_USE_IDLE == TLS_ENABLE
            deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
            deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
        } else if (phase == OPEN_WAIT) {
            phase = BACKOFF;
#if TLS_USE_IDLE == TLS_ENABLE
            const uint16_t now16 = tinyls::kernel::idle_prev_ms;
#else
            const uint16_t now16 = (uint16_t)tinyls::now;
#endif
            // Swap the stored nibbles without sampling time. Select this
            // backoff before the conditional XOR, which changes future state
            // only. Bit 4 belongs to the swapped byte's upper nibble.
            uint8_t mixed = (uint8_t)((randomByte << 4) | (randomByte >> 4));
            const uint8_t pause = (uint8_t)((mixed & 3u) + ((mixed >> 2) & 1u));
            if (mixed & 16u)
                mixed ^= pause;
            randomByte = mixed;
            deadline16 = (uint16_t)(now16 + pause);
            // Zero adds no mandatory wait. TX may retry as soon as its normal
            // phase logic permits, including later in this same NET pass.
        } else if (phase == FINISH_WAIT) {
            phase = RX_TURN;
#if TLS_USE_IDLE == TLS_ENABLE
            deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
            deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
            cursor = 0; // FINISH already gave reverse permission.
        }
        break;
    case OK:
        if (phase == OPEN_WAIT) {
            cursor = 0;
            phase = TX_NEXT;
        } else if (phase == TX_ACK) {
            ++cursor;
            phase = TX_NEXT;

        } else if (phase == FINISH_WAIT) {
            cursor = 0;
            phase = IDLE;
#if TLS_USE_IDLE == TLS_ENABLE
            deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
            deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
        }
        break;
    case RETRY:
        if (phase == TX_ACK) {
#if TLS_USE_IDLE == TLS_ENABLE
            if ((int16_t)(tinyls::kernel::idle_prev_ms - deadline16) < 0)
#else
            if ((int16_t)((uint16_t)tinyls::now - deadline16) < 0)
#endif
                sendRecord(false);
            else
                fault(true);
        }
        break;
    case FINISH:
        if (phase == RX_TURN) {
            cursor = 0;
            if ((flags & HAS_DIRTY) && nextDirty(0) != 255) {
                NLS_SERIAL.write(ASK);
                NLS_SERIAL.write((uint8_t)~ASK);
                phase = TX_NEXT; // OK+ASK and pre-granted reverse turn.
            } else {
                refreshDirty();
                NLS_SERIAL.write(OK);
                NLS_SERIAL.write((uint8_t)~OK);
                phase = IDLE;
            }
            if (phase != TX_NEXT) {
#if TLS_USE_IDLE == TLS_ENABLE
                deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
                deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
            }
        }
        break;
    default:
        break;
    }
}
