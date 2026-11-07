# MTW NetLoopScheduler Protocol

## Transport and execution

DUO uses one point-to-point native UART link, 8N1, with logical half duplex.
One participant owns a shared-variable turn; the other returns short responses.
The core ISR moves bytes between UART registers and native rings. NET parses
and enqueues in main context, before the TLS IDLE phase. It never waits for an
entire record, a reply, or physical TX completion.

## Wire formats

| Frame | Bytes |
|---|---|
| Shared record | ID, ~ID, VALUE, SUM8(VALUE) |
| Short command | CMD, ~CMD |
| HELLO / HELLO_REPLY | CMD, ~CMD, STATUS, ~STATUS |

No leading null byte, COBS, escaping or payload transformation is used.
ID is table index + 1, in 1..254; FINISH is 00/FF. VALUE length comes from the
matching table entry: 1, 2, 4, or 8 bytes. VALUE uses the AVR storage representation.
SUM8 is the unsigned sum of VALUE bytes modulo 256. ID and ~ID are NOT included.
For example, ID 2 and VALUE bytes 34, 12 produce `02 FD 34 12 46`.

## Commands

| Command | Code | Inverse | Purpose |
|---|---|---|---|
| FINISH | 00 | FF | End a shared turn |
| ASK | E0 | 1F | Request a shared turn |
| OK | E2 | 1D | Grant a turn, accept a record, or acknowledge FINISH |
| RETRY | E4 | 1B | Request current record again |
| HELLO | E6 | 19 | Handshake, liveness, reset/full-transfer request |
| HELLO_REPLY | E8 | 17 | Reply and acknowledge HELLO actions |

When assigning a new command code, choose an unused byte whose inverse is
not assigned to any command. An inverse byte is validation, not a separate
command code. Use 00 for FINISH and 80..FE for other commands; their inverses
are FF and 7F..01 respectively. This convention allows 128 command codes,
with 6 currently assigned and 122 available. It does not reserve those bytes
inside ID or VALUE. Codes are assigned manually as ordinary constants;
there is no code generator, validation macro, or compile-time allocation check.

## Normal exchange

1. A local commit sets dirty and HAS_DIRTY.
2. If the link is online and idle, the owner sends ASK and waits for OK.
3. It finds the next dirty entry, writes one record to native TX, clears that
   entry's dirty bit, and waits for OK or RETRY.
4. The receiver validates ID/~ID, gathers only this scalar into 8-byte scratch,
   and checks SUM8. Good VALUE is applied and OK is enqueued. Bad SUM8 is
   rejected and RETRY is enqueued. No partial VALUE is applied.
5. OK advances the cursor. RETRY rereads the same current application variable;
   it does not replay a retained snapshot. A new commit during ACK waiting
   survives unless the current value is sent again on RETRY.
6. When the forward scan ends, FINISH closes the turn. The receiver returns OK
   if it has no work, or ASK to acknowledge FINISH and take the pre-granted
   reverse turn without another OK round trip.

An already-passed dirty entry is visited on a later turn. The scan never wraps
within one turn, so a continuous producer cannot make a turn infinite.
Received values do not automatically set local dirty. Multi-variable updates
are not atomic and repeated assignments after lost acknowledgements are possible.

## Parser boundaries and recovery

Normal RX consumes at most one complete command/record and at most 11 bytes per
NET phase. The first successful read marks I/O activity once. Raw command bytes
inside VALUE are legal and never treated as markers. In an open RX shared turn,
a nonzero header is a shared ID, not an injected system/user command.

After validating ID/inverse, RX caches the fixed destination and length. A
remaining-byte counter also indexes the existing scalar scratch in reverse.
The first VALUE byte is handled directly; a short inner loop consumes only
currently available VALUE bytes. An empty read returns immediately. APPLY
reads the scratch in reverse index order to restore the original VALUE order.
The maximum validated length and stopping after one record enforce the
11-byte bound without a separate per-byte budget counter.

A bad ID/inverse, invalid ID, or uncertain stream boundary abandons the exchange
and enters bounded RESYNC draining. There is no scan for HELLO inside VALUE.
A missing OK/FINISH/record eventually expires the common deadline. Outstanding
TX dirty is restored; HELLO with RESET reestablishes the protocol boundary.
RESYNC drains only one available() snapshot per phase, not an unbounded stream.

One 16-bit deadline uses cached TLS time. The default wait is 1600 ms; the
supported setting is 1..32767 ms. RETRY does not renew an unaccepted record's deadline. Idle expiry
sends HELLO for liveness. A valid incoming HELLO/response renews the applicable
idle wait, but unrelated keepalives do not postpone an outstanding record ACK.
An offline peer is periodically probed; recovery retains pending current state.

Applications read the public pseudorandom byte through `netls::random()`;
NLS also uses it for collision backoff. Reads do not change it. At the end of
NET initialization, `micros() >> 2` supplies a byte; if its bit 0 is zero,
NET inverts the byte. This is the only time sample used for random state.
On crossed ASK, NET swaps the stored nibbles and calculates the nonblocking
pause as `(byte & 3) + ((byte >> 2) & 1)`, or 0..4 ms. If bit 4 of the swapped
byte is set, NET XORs that byte with the already selected pause and stores it.
The XOR does not change the current pause. Zero adds no mandatory wait.

`NLS_RANDOM` defaults to `COLLISION`: inactive NET passes contain no random
update code. Define `NLS_RANDOM NETLESS` before the library include to also
increment the byte modulo 256 and swap its nibbles on each completely inactive
NET pass. An outstanding response wait, backoff, partial RX, dirty work or
actual byte reception/transmission is not an inactive pass. Neither policy
samples `micros()` on collisions or ordinary NET passes. The deterministic
transformations can repeat or converge to identical states; neither policy
guarantees that repeated collisions eventually separate.

## HELLO status and startup

| Bit | Name | Meaning |
|---|---|---|
| 0 | RUN | Local initial handshake has previously completed |
| 1 | SEEN | Historical peer RUN has been observed |
| 2 | FULL | Request a full shared-table pass |
| 3 | RESET | Abandon uncertain outstanding exchange/parser state |
| 4..7 | Reserved | No current meaning |

RESET is a protocol action, not a hardware reset. HELLO_REPLY echoes accepted
FULL/RESET actions. Startup policy runs once per local init. COLD performs no
initial full sync; BROADCAST marks all; RECOVER requests a full pass;
BROADCAST_RECOVER selects recovery when the peer remembers this participant,
otherwise broadcast. History is SRAM-only and disappears if both restart.

## State and memory

Internal flags: HAS_DIRTY, ONLINE, HELLO_DONE, PEER_SEEN, BOOT_DONE.
Phase,
RX stage, cursor, partial header/status, object index, RX position and RX sum
are byte-sized. The receiver has 8-byte scalar scratch, a cached length and
destination pointer, and one 16-bit deadline. One additional byte stores the public random value also used for collision backoff. Two small configuration-sized SRAM tables
replace variable bit shifts and repeated bit-search iterations.
There is no TX snapshot, slot buffer, sequence number or runtime command table.
Command allocation follows the documented convention only.

For N entries and 16-bit pointers, descriptor/dirty/state storage is about
`3 * N + ceil(N / 8) + 26 + min(N, 8) + 2^min(N, 4)` bytes, excluding VALUE, TLS, native Serial and stack.
The tables contain min(N,8) mask bytes and 2^min(N,4) search entries. Two
configuration masks limit small-N searches and describe the tail byte. Together
with the three RX cache bytes, added persistent storage is at most 29 bytes.
The compiler can eliminate unused constants/data. Tables are filled once by
init(), which also restores them on repeated initialization. One common lookup
algorithm handles every size; no small/large configuration dispatch is used in NET. Internal names are not stable public API.

## Extension directions

Raw blocks around 48..56 bytes, application/system commands, user commands and
flags/events are draft directions only. They have no assigned wire formats,
public handlers or allocated buffers here. `netls` exposes the application random-byte getter in this version.
A new transfer type must explicitly select a parser context; it cannot simply
insert a command into an open shared turn. Old peers do not negotiate features
and reject unknown command headers. Both ends must use compatible firmware.

SUM8 and inverse pairs detect only a subset of possible faults; they are not
cryptographic protection, exactly-once delivery, or an event-log guarantee.
