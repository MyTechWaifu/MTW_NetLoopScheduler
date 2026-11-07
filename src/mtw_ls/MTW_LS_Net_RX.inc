// SPDX-License-Identifier: Apache-2.0
/*
 Consume at most one complete record/command and at most eleven bytes on the
 normal path (ID, inverse, largest scalar, SUM8). The first successful read
 sets the IDLE-skip bit before entering the loop; following bytes do not repeat
 that operation or test an extra first-byte flag. A negative int result is
 checked before conversion, so data byte 0xFF stays distinct from an empty ring.

 HEADER is interpreted only at a proven record boundary. VALUE bytes are raw,
 including command values and zero. APPLY happens only after SUM8 succeeds.
 The validated descriptor fixes VALUE at at most eight bytes. One decoded
 record stops this phase, so HEADER + inverse + VALUE + SUM8 is at most eleven
 reads without a redundant byte-budget decrement on every byte. An empty read
 returns immediately; no complete-body wait or second record buffer is used.
 RESYNC drains one available() snapshot instead of following an unbounded
 producer: newly arriving garbage cannot keep this NET phase running forever.
*/
if (phase == RESYNC) {
    int ready = NLS_SERIAL.available();
    if (ready > 0 && NLS_SERIAL.read() >= 0) {
#if TLS_USE_IDLE == TLS_ENABLE
        tinyls::kernel::state_mask |= tinyls::kernel::STATE_SKIP_DELTA;
#endif
        while (--ready > 0) {
            (void)NLS_SERIAL.read();
        }
    }
} else {
    int raw = NLS_SERIAL.read();
    if (raw >= 0) {
#if NLS_RANDOM == NETLESS
        randomIdle = false;
#endif
#if TLS_USE_IDLE == TLS_ENABLE
        tinyls::kernel::state_mask |= tinyls::kernel::STATE_SKIP_DELTA;
#endif
        bool complete = false;
        do {
            const uint8_t b = (uint8_t)raw;
            switch (rxStage) {
            case HEADER:
                rxHeader = b;
                // Command-valued IDs are legal in an opened shared turn. VALUE
                // is never scanned for markers; only the parser's boundary counts.
                if (phase == RX_TURN && b != FINISH)
                    rxStage = ID_INVERSE;
                else if (b == ASK || b == OK || b == RETRY || b == HELLO || b == HELLO_REPLY ||
                         b == FINISH)
                    rxStage = COMMAND_INVERSE;
                else {
                    fault(false);
                    complete = true;
                }
                break;
            case COMMAND_INVERSE:
                if ((uint8_t)(b ^ rxHeader) != 255) {
                    fault(false);
                    complete = true;
                } else if (rxHeader == HELLO || rxHeader == HELLO_REPLY)
                    rxStage = STATUS_BYTE;
                else {
                    rxStage = HEADER;
#include "MTW_LS_Net_Command.inc"
                    complete = true;
                }
                break;
            case STATUS_BYTE:
                rxStatus = b;
                rxStage = STATUS_INVERSE;
                break;
            case STATUS_INVERSE:
                if ((uint8_t)(b ^ rxStatus) != 255) {
                    fault(false);
                    complete = true;
                } else {
                    rxStage = HEADER;
#include "MTW_LS_Net_Hello.inc"
                    complete = true;
                }
                break;
            case ID_INVERSE: {
                const uint8_t index = (uint8_t)(rxHeader - 1u);
                if ((uint8_t)(b ^ rxHeader) == 255) {
                    rxObject = index < (&nls_shared_count ? nls_shared_count : 0u) ? index : 255u;
                }
                if ((uint8_t)(b ^ rxHeader) != 255 || rxObject == 255u) {
                    fault(false);
                    complete = true;
                } else {
                    // Cache the descriptor once; neither field changes at run time.
                    rxLength = nls_shared_objects[rxObject].size;
                    rxTarget = nls_shared_objects[rxObject].ptr;
                    rxPosition = rxLength;
                    rxSum = 0;
                    rxStage = VALUE_BYTES;
                }
                break;
            }
            case VALUE_BYTES:
                // Store backwards so the remaining-byte countdown also gives
                // the scratch index. APPLY restores normal VALUE byte order.
                // First byte is handled directly; the short loop reads only
                // bytes already present and stops on an empty native read().
                rxValue[--rxPosition] = b;
                rxSum = (uint8_t)(rxSum + b);
                if (rxPosition == 0) {
                    rxStage = VALUE_SUM;
                } else {
                    for (;;) {
                        raw = NLS_SERIAL.read();
                        if (raw < 0) {
                            complete = true;
                            break;
                        }
                        rxValue[--rxPosition] = (uint8_t)raw;
                        rxSum = (uint8_t)(rxSum + (uint8_t)raw);
                        if (rxPosition == 0) {
                            rxStage = VALUE_SUM;
                            break;
                        }
                    }
                }
                break;
            case VALUE_SUM:
                rxStage = HEADER;
                complete = true;
                if (rxSum != b) {
                    NLS_SERIAL.write(RETRY);
                    NLS_SERIAL.write((uint8_t)~RETRY);
                    // No progress: preserve this record's original deadline.
                } else {
                    uint8_t *destination = rxTarget;
                    const uint8_t *source = rxValue + rxLength;
                    uint8_t remaining = rxLength;
                    do {
                        *destination++ = *--source;
                    } while (--remaining);
                    flags |= (ONLINE | PEER_SEEN);
                    NLS_SERIAL.write(OK);
                    NLS_SERIAL.write((uint8_t)~OK);
#if TLS_USE_IDLE == TLS_ENABLE
                    deadline16 = (uint16_t)(tinyls::kernel::idle_prev_ms + NLS_TIMEOUT_MS);
#else
                    deadline16 = (uint16_t)((uint16_t)tinyls::now + NLS_TIMEOUT_MS);
#endif
                }
                break;
            default:
                fault(false);
                complete = true;
                break;
            }
        } while (!complete && (raw = NLS_SERIAL.read()) >= 0);
    }
}
