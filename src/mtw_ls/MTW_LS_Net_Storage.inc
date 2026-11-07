// SPDX-License-Identifier: Apache-2.0
/*
 One logical half-duplex link. Data go directly from shared storage to the
 native serial object. Only the untrusted RX VALUE needs scratch (8 bytes).
 Cursors and parser fields are bytes; no TX snapshot, slots, target masks,
 sequence number, public diagnostics, heap, or user command dispatcher.
*/
enum Command : uint8_t {
    FINISH = 0x00,
    ASK = 0xE0,
    OK = 0xE2,
    RETRY = 0xE4,
    HELLO = 0xE6,
    HELLO_REPLY = 0xE8
};

enum Phase : uint8_t {
    IDLE,
    OPEN_WAIT,
    TX_NEXT,
    TX_ACK,
    FINISH_WAIT,
    RX_TURN,
    BACKOFF,
    HELLO_WAIT,
    RESYNC
};
enum RxStage : uint8_t {
    HEADER,
    COMMAND_INVERSE,
    STATUS_BYTE,
    STATUS_INVERSE,
    ID_INVERSE,
    VALUE_BYTES,
    VALUE_SUM
};
enum HelloStatus : uint8_t { RUN = 1, SEEN = 2, FULL = 4, RESET = 8 };
uint8_t flags;
// One persistent byte seeded at init and mixed on crossed ASK. NETLESS also
// advances it on inactive NET passes; public reads always leave it unchanged.
static uint8_t randomByte;
static uint8_t phase, rxStage, cursor;
static uint8_t rxHeader, rxStatus, rxObject, rxPosition, rxSum;
static uint8_t rxValue[8];
/* Outstanding HELLO action flags. Replies echo accepted FULL/RESET, so a late
   ordinary HELLO reply cannot accidentally satisfy a recovery/reset request. */
static uint8_t helloWanted;
static uint16_t deadline16;

// Three cached descriptor bytes on ordinary AVR: length and 16-bit pointer.
static uint8_t rxLength;
static uint8_t *rxTarget;

// SHARED_CONFIG owns the exact-sized lookup arrays. init() fills them once.
// No table storage exists when SHARED_CONFIG is absent.
