// SPDX-License-Identifier: Apache-2.0
#pragma once
/*
 NET is always part of NetLoopScheduler. Ordinary TLS features retain their
 original switches; no TLS backend file needs a NET-specific option.

 DUO is one peer on one native hardware UART. MANY reserves a future mode;
 it does not allocate masks, slots, routing tables or additional links here.

*/
#define NLS_MODE_DUO 0
#define NLS_MODE_MANY 1
#define NLS_COLD 0
#define NLS_BROADCAST 1
#define NLS_RECOVER 2
#define NLS_BROADCAST_RECOVER 3

#ifndef NLS_MODE
#define NLS_MODE NLS_MODE_DUO
#endif
#ifndef NLS_SERIAL
#define NLS_SERIAL Serial1
#endif
#ifndef NLS_BAUD
// One baud coefficient for every supported AVR/core. User override wins.
#define NLS_BAUD (F_CPU / 24UL)
#endif
#ifndef NLS_POLICY
#define NLS_POLICY NLS_COLD
#endif
// Random state always advances on crossed ASK. NETLESS also advances it on
// completely inactive NET passes. The default compiles out that idle work.
#define COLLISION 0
#define NETLESS 1
#ifndef NLS_RANDOM
#define NLS_RANDOM COLLISION
#endif

#ifndef NLS_TIMEOUT_MS
#define NLS_TIMEOUT_MS 1600U
#endif

#if NLS_MODE != NLS_MODE_DUO
#error "NLS MVP supports NLS_MODE_DUO; NLS_MODE_MANY is reserved"
#endif
#if NLS_POLICY < NLS_COLD || NLS_POLICY > NLS_BROADCAST_RECOVER
#error "NLS_POLICY is invalid"
#endif
#if NLS_BAUD < 1
#error "NLS_BAUD must be positive"
#endif
#if NLS_TIMEOUT_MS < 1 || NLS_TIMEOUT_MS > 32767
#error "NLS_TIMEOUT_MS must be in 1..32767 ms for the 16-bit deadline"
#endif
#if defined(SERIAL_RX_BUFFER_SIZE) && SERIAL_RX_BUFFER_SIZE < 64
#error "NLS requires a native RX ring of at least 64 bytes"
#endif
#if defined(SERIAL_TX_BUFFER_SIZE) && SERIAL_TX_BUFFER_SIZE < 64
#error "NLS requires a native TX ring of at least 64 bytes"
#endif
