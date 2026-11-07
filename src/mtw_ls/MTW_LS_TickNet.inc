// SPDX-License-Identifier: Apache-2.0
/*
 One NET phase, after PERIODIC/DELAYED and before stock TLS IDLE/POSTED/POLL.
 The phase body lives directly in tinyls::tick(), like the TLS phase bodies.
 State and protocol helpers belong to netls::kernel; the UART core is unchanged.
 Work is bounded by one decoded record/command, plus one outgoing step.
 Incoming data and outstanding operations are serviced even without dirty.
*/
{
    using namespace ::netls::kernel;
#if NLS_RANDOM == NETLESS
    // Remember whether the pass started with no protocol or dirty work.
    // RX clears this local eligibility on its first successful native read.
    bool randomIdle = phase == IDLE && rxStage == HEADER && !helloWanted &&
                      !(flags & HAS_DIRTY);
#endif

    // Send the initial status before receiving a simultaneous peer HELLO.
    // COLD does not itself mark variables or request a snapshot.
    if (!(flags & HELLO_DONE) && phase == IDLE) {
        sendHello();
    }

#include "MTW_LS_Net_RX.inc"
#include "MTW_LS_Net_TX.inc"
#if NLS_RANDOM == NETLESS
    // A timeout-triggered HELLO leaves IDLE, so it is not an inactive pass.
    // Outstanding waits, including BACKOFF and TX_ACK, never advance here.
    if (randomIdle && phase == IDLE) {
        uint8_t mixed = (uint8_t)(randomByte + 1u);
        randomByte = (uint8_t)((mixed << 4) | (mixed >> 4));
    }
#endif
}
