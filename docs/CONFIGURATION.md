# MTW NetLoopScheduler Configuration

## Configuration scope

Set definitions before the first `<MTW_NetLoopScheduler.h>` include. Additional
translation units must use the same settings as the implementation owner.
For example:

```cpp
#define NLS_SERIAL Serial1
#define NLS_BAUD 666666UL
#define NLS_POLICY NLS_COLD
#define NLS_RANDOM NETLESS
#define NLS_TIMEOUT_MS 1600U
#include <MTW_NetLoopScheduler.h>
```

| Option | Default | Meaning |
|---|---|---|
| NLS_MODE | NLS_MODE_DUO | Two MCUs, one link; MANY is not implemented |
| NLS_SERIAL | Serial1 | Core hardware Serial object |
| NLS_BAUD | F_CPU / 24UL | Common coefficient for every supported AVR family |
| NLS_POLICY | NLS_COLD | Startup synchronization policy |
| NLS_RANDOM | COLLISION | COLLISION: crossed ASK only; NETLESS: also inactive NET passes |
| NLS_TIMEOUT_MS | 1600U | Common response/liveness wait, 1..32767 ms |

NET and SUM8 are always enabled. There is no SUM8 switch, autosync timer,
standalone NET enable switch, or runtime setup API. Existing TLS subsystem
switches retain their meanings; see [TLS configuration](https://github.com/MyTechWaifu/MTW_TinyLoopScheduler/blob/main/docs/CONFIGURATION.md).

## Baud rate

8/12/16/20/24 MHz correspond to requested baud rates
333333/500000/666666/833333/1000000. Both ends must select the same actual
baud. Override the default for mixed clocks. Core/hardware divider rounding
and oscillator accuracy still apply.

## Startup policy

| Policy | Shared action after initial HELLO |
|---|---|
| NLS_COLD | No full startup transfer; subsequent commits are sent normally |
| NLS_BROADCAST | Mark all local variables dirty |
| NLS_RECOVER | Request a full table pass from the peer |
| NLS_BROADCAST_RECOVER | Recover if the peer remembers this participant; otherwise broadcast |

COLD still performs HELLO and liveness checks. Startup policy runs once per
local init, not on every repeated HELLO. Peer history lives in SRAM and is lost
when both MCUs restart. Application code decides which side owns each value.

## Deadline and collision backoff

One 16-bit deadline is reused across mutually exclusive protocol states. NET
uses the TLS cached time, without a second millis() read. Call tick frequently;
16-bit signed-delta comparisons require servicing gaps shorter than 32768 ms.
An unaccepted RETRY does not extend the outstanding record's deadline.

Crossed ASK requests use a nonblocking pause of 0..4 ms, calculated from the
same byte that applications read through `netls::random()`. A zero pause adds
no mandatory wait: the ordinary TX path may retry in the current NET pass.

## Random-byte policy

The public `netls::random()` API exposes a stored pseudorandom byte for the
application. `NLS_RANDOM` controls when the library changes that byte; it does
not change what a read does. The same byte also supplies NLS collision backoff.

| Setting | Default | Update events |
|---|---|---|
| `NLS_RANDOM COLLISION` | Yes | Crossed ASK |
| `NLS_RANDOM NETLESS` | No | Crossed ASK and completely empty NET passes |

Choose one definition before the include, or omit it to keep `COLLISION`.
The default compiles out the empty-pass eligibility and random-update code.
Select `NETLESS` when application code needs the stored byte to change during
periods without network work. It does not guarantee a new value on every read
or a fixed update frequency. No additional persistent random-state byte is
allocated by this choice.

### Initialization

At the end of NET initialization, after starting the selected Serial object:

1. Sample `micros()` once.
2. Take `uint8_t(microsValue >> 2)`.
3. If that byte's bit 0 is zero, invert the whole byte.
4. Store the result for application reads and later collision handling.

The checked bit is original `micros()` bit 2. On classic 16 MHz AVR, the two
lower bits are zero because `micros()` advances in 4 us steps. Other cores may
have different time resolution; the expression is the same and touches no
hardware timer registers.

### Completely empty NET pass

With `NETLESS`, increment the stored byte modulo 256, swap its upper and lower
four-bit halves, and save it. A pass is eligible only if it starts without
protocol or dirty work, no byte is consumed, and it ends idle. An outstanding
ACK/HELLO wait, backoff, partial RX record or actual NET transmission is not
an empty pass. `COLLISION` contains none of this extra empty-pass code.

### Crossed ASK

Both policies use the same transformation:

1. Swap the stored byte's upper and lower four-bit halves.
2. Calculate `pause = (byte & 3) + ((byte >> 2) & 1)` in milliseconds.
3. If bit 4 of the swapped byte is set, XOR that byte with `pause`.
4. Store the resulting byte and arm the existing deadline with the previously
   calculated pause. Do not recalculate the pause after XOR.

The updates are deterministic and do not promise uniform or independent
random samples. Getter calls do not advance the byte. Neither collisions nor
empty passes call `micros()`; initialization supplies the sole time sample.
The selected core must provide working `millis()` and `micros()` timekeeping.
No hardware timer is configured by NLS.

## UART and IDLE

Reserve the selected port exclusively for NET. Native RX and TX rings must be
at least 64 bytes each. Application console/debug bytes must not share it.
Successful RX extraction or serial TX enqueue sets the existing TLS IDLE-skip
bit for that pass. Empty reads and waiting for OK do not permanently disable
sleep. Native UART and timer interrupts provide normal wakeups.
