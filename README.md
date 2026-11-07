# MTW NetLoopScheduler
[![DOI](https://zenodo.org/badge/1449011313.svg)](https://doi.org/10.5281/zenodo.24027638)

MTW NetLoopScheduler is a lightweight cooperative task scheduler with automatic
UART data exchange for projects built around two AVR microcontrollers. Its
simple API combines periodic, delayed and event-driven tasks with shared
variables: application code updates values locally while the library handles
transmission, acknowledgements and retries in the background.

Scheduling is based on
[MTW TinyLoopScheduler](https://github.com/MyTechWaifu/MTW_TinyLoopScheduler).
Use one MCU to read a sensor and another to update a display, or split control
and interface tasks between two chips without writing the UART exchange code
in each application.

Call one scheduler entry point from the ordinary Arduino loop:

```cpp
tinyls::tick();
```

The library is header-only and procedural. Storage is static, shared variables
are ordinary application variables, and UART interrupts and ring buffers belong
to the installed Arduino core.

## Status

Current version: **0.1.2**.

The current network mode is **DUO**: two microcontrollers connected by one
logical half-duplex UART link. Each shared variable is acknowledged separately.
SUM8 is always enabled and covers VALUE bytes only.

## Hardware and platform support

Designed for AVR microcontrollers with an Arduino-compatible core, a hardware
Serial interface, working millis()/micros(), and sufficient Flash/SRAM. This includes classic AVR,
megaAVR 0-series, and AVR Dx when supported by the selected core.

Hardware-tested on:

- ATmega2560;
- ATmega328PB;
- AVR128DB32.

These are reference devices, not a closed list of supported AVR chips.
Other AVR targets require a compatible hardware Serial API and native RX/TX
ring buffers of at least 64 bytes each. Non-AVR NET backends are not provided.

### UART assignment

| Available hardware UARTs | NET | Console |
|---|---|---|
| Two or more | UART1 / `Serial1` by default | UART0 remains available to the application |
| One | Define `NLS_SERIAL` as `Serial` | The UART is used exclusively for NET |

The library initializes only the selected NET port. It does not initialize a
console. On USB-native boards the core may expose `Serial` as USB and
`Serial1` as a hardware UART; use the core's actual object names.

Connect TX to the other MCU's RX, RX to its TX, and share GND. No additional
handshake pins are required. Use compatible I/O levels.

## Main features

- PERIODIC, DELAYED, POSTED, POLL, and optional PARTED callbacks from TLS;
- shared variables declared with one static `SHARED_CONFIG` table;
- `shared::set()` and `shared::commit()`;
- `netls::random()` to read a pseudorandom byte for application code;
- automatic transmission in the nearest available NET phase;
- native hardware UART transmission and reception in the background;
- raw VALUE bytes, without escaping or byte stuffing;
- mandatory VALUE-only SUM8 and per-variable OK/RETRY;
- startup policies and automatic connection recovery;
- configurable UART, baud rate, and timeout;
- static memory, no heap allocation, and no custom serial driver.

The scheduler APIs are summarized below. Full scheduling documentation is
maintained in [MTW TinyLoopScheduler](https://github.com/MyTechWaifu/MTW_TinyLoopScheduler).

## Installation

### Arduino Library Manager

1. Open Arduino IDE.
2. Open **Sketch → Include Library → Manage Libraries...**
3. Search for **MTW NetLoopScheduler**.
4. Select the library and click **Install**.

```cpp
#include <MTW_NetLoopScheduler.h>
```

Open the supplied sketches under **File → Examples → MTW NetLoopScheduler**.

### PlatformIO

Install the library from the
[PlatformIO Registry](https://registry.platformio.org/libraries/techiewaifu/mtw-netloopscheduler).
Add it to your project's `platformio.ini`:

```ini
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino

lib_deps =
    techiewaifu/mtw-netloopscheduler
```

Then include `<MTW_NetLoopScheduler.h>` in `src/main.cpp`.
Ready-to-build projects are available in
[platformio-examples/](platformio-examples/README.md). Each example has its own
`platformio.ini` and `src/main.cpp`; open the individual example directory.

## Minimal shared-variable example

```cpp
#include <MTW_NetLoopScheduler.h>

// An ordinary variable declared before the shared table.
uint32_t counter = 0;

SHARED_CONFIG(
    (temperature, uint16_t, 20),  // Declare, initialize and bind.
    (enabled, bool),             // Declare as false and bind.
    (counter)                    // Bind the existing variable.
);

static void updateTemperature()
{
    shared::set(temperature, temperature + 1);
}

void setup()
{
    tinyls::init();
    tinyls::every(updateTemperature, 1000u);
}

void loop()
{
    tinyls::tick();
}
```

Use the same table order and types on the other MCU. The receiving application
reads `temperature` normally; NET updates its storage after checksum validation.
Only the source MCU needs the `updateTemperature` callback. The table shows
all three declaration forms; `enabled` and `counter` are also available as
ordinary variables, but this minimal callback changes only `temperature`.

## Shared-variable API

| Configuration entry | Meaning |
|---|---|
| `(name)` | Bind an existing mutable variable |
| `(name, type)` | Declare a zero-initialized variable and bind it |
| `(name, type, initial_value)` | Declare, initialize, and bind a variable |

Forms may be mixed. A namespace is optional. Declare the table once at file
scope. Both MCUs must use the same entry order and sizes. Wire IDs are assigned
as table index + 1, in the range 1..254. Supported VALUE sizes are 1, 2, 4, and
8 bytes. Configuration errors are checked at compile time.

```cpp
shared::set(temperature, 25);

enabled = true;
shared::commit(enabled);
```

`set()` assigns and marks dirty. `commit()` marks the current value dirty.
Repeated commits coalesce; there is no periodic autosync interval. Direct
assignment alone does not schedule transmission. Received values do not
implicitly create an outgoing commit.

Use these APIs in main context, including scheduler callbacks. Do not call them
or modify shared storage from hardware ISRs or concurrent execution contexts.
For the normal single-writer pattern, one MCU updates a variable and the other
reads it. This is state synchronization, not a queue of every intermediate event.

## Scheduler integration

| API | Purpose |
|---|---|
| `tinyls::every()` | Repeating callback |
| `tinyls::after()` | One-shot delayed callback |
| `tinyls::post()` | Callback dispatched by the POSTED phase |
| `tinyls::poll()` | Callback visited on each scheduler pass |
| PARTED | Split work into short cooperative steps |
| `tinyls::postISR()` | Defer a callback from a supported hardware ISR |

Pass order: cached time → deferred ISR work → PERIODIC → DELAYED → **NET** →
IDLE → POSTED → POLL. The application keeps its ordinary `loop()`.

NET incrementally parses incoming bytes and advances one outgoing step. It
never waits for a reply or physical UART completion. Successful ring reads and
outgoing serial writes suppress IDLE for that pass; merely waiting for OK does
not permanently disable sleep.

## Configuration

Define options before the first `<MTW_NetLoopScheduler.h>` include.

| Option | Default | Purpose |
|---|---|---|
| `NLS_MODE` | `NLS_MODE_DUO` | Two-MCU network mode |
| `NLS_SERIAL` | `Serial1` | Native hardware Serial object |
| `NLS_BAUD` | `F_CPU / 24UL` | One baud coefficient for all supported AVR cores |
| `NLS_POLICY` | `NLS_COLD` | Startup shared-variable policy |
| `NLS_RANDOM` | `COLLISION` | Update the application random byte on collisions; `NETLESS` also updates on empty NET passes |
| `NLS_TIMEOUT_MS` | `1600U` | Common response/liveness deadline, 1..32767 ms |

| CPU clock | Default baud |
|---:|---:|
| 8 MHz | 333333 |
| 12 MHz | 500000 |
| 16 MHz | 666666 |
| 20 MHz | 833333 |
| 24 MHz | 1000000 |

Both ends must use the same actual baud rate. For mixed clocks, explicitly set
a common `NLS_BAUD`. The core selects the hardware divider and may round the request.

Startup policies: `NLS_COLD`, `NLS_BROADCAST`, `NLS_RECOVER`, and
`NLS_BROADCAST_RECOVER`. See [configuration](docs/CONFIGURATION.md).

## Random byte for application code

`netls::random()` returns the current pseudorandom `uint8_t` byte for the
application. It can supply a startup delay, select an application variation,
or provide a small changing value without a separate application generator.
NLS also uses this same byte internally when simultaneous ASK requests collide.

Read it after `tinyls::init()`. A read returns the stored value; it does not
advance the byte, call `micros()`, or start network work. Consecutive reads may
therefore return the same value. The byte's numeric range is 0..255; the update
mechanism does not promise a uniform distribution or independent samples.

| `NLS_RANDOM` value | When the stored byte changes |
|---|---|
| `COLLISION` (default) | On a crossed ASK; no extra random work on empty NET passes |
| `NETLESS` | On a crossed ASK and on every completely empty NET pass |

To make the byte change more often during otherwise inactive network passes:

```cpp
#define NLS_RANDOM NETLESS
#include <MTW_NetLoopScheduler.h>
```

With no definition, `COLLISION` is selected. Both choices are compile-time
settings. An empty pass has no active exchange or response wait, partial RX,
dirty work, or NET byte I/O. Waiting for an acknowledgement or backoff is not
an empty pass. The update rate therefore depends on network/application work;
`NETLESS` does not add a periodic random timer or change the value on reads.

Initialization samples `micros() >> 2` once and inverts the resulting byte if
its bit 0 is zero. On a collision, NLS swaps the byte's nibbles, selects the
0..4 ms backoff, then conditionally XORs the byte with that pause when swapped
bit 4 is set. On an empty `NETLESS` pass, it increments and swaps the byte.
See [configuration](docs/CONFIGURATION.md) for the exact update rules.

## Approximate CPU cost

Reference conditions: ATmega328PB at 16 MHz, MiniCore 3.1.3, AVR GCC 7.3,
`-Os/-flto`, IDLE enabled, 16 configured shared variables and the default
`NLS_RANDOM COLLISION` policy. These are **complete scheduler tick** costs,
including native Serial calls, with UART ISR execution and wire time excluded.

The planning figures below add **10% engineering headroom** to the reference
instruction counts, then round upward to the next ten cycles. This allowance
is not a measured statistical error or a worst-case bound for other cores,
compiler versions, table sizes or application callbacks.

| Complete tick with operation | Planning guide, CPU cycles |
|---|---:|
| No traffic, no dirty variables | ~270 |
| Waiting for an acknowledgement, RX empty | ~270 |
| Send one 8-bit variable | ~830 |
| Send one 32-bit variable | ~1160 |
| Receive one 8-bit variable and enqueue OK | ~820 |
| Receive one 32-bit variable and enqueue OK | ~1020 |

`NETLESS` performs extra eligibility checks and idle updates. In the same
reference build, the empty-tick instruction count rises from 244 to 270 cycles;
allow approximately **300 cycles** with the same headroom and rounding.

The receive cases start with a complete record already in the native RX ring.
A fragmented record spans several ticks; ISR work, application callbacks and
wire time must be budgeted separately. These instruction-model figures are
CPU-cost guides, not end-to-end latency guarantees.

## Approximate throughput

Representative sustained exchange with batches of 128 variables and a short
foreground loop, including protocol acknowledgements. Figures are approximate
orientation values, not guaranteed rates for arbitrary application workloads.

| Reference target | Baud | 8-bit values/s | 32-bit values/s |
|---|---:|---:|---:|
| ATmega328PB, 16 MHz | ~667000 | ~6200 | ~4800 |
| AVR128DB32, 24 MHz | 1000000 | ~9300 | ~7200 |

## Approximate memory usage

On ordinary AVR targets with 16-bit pointers, NET/shared bookkeeping is about
`3 * N + ceil(N / 8) + 26 + min(N, 8) + 2^min(N, 4)` bytes, plus minor configuration constants that the
compiler may retain. Allow approximately **60 bytes for 4 variables** or
**100 bytes for 16 variables**. SHARED_CONFIG sizes the mask and bit-search tables from N. Their maxima are
8 and 16 bytes; two configuration masks and three cached RX descriptor bytes
bring the RX/lookup optimization storage to at most 29 extra bytes. The
public random state adds one more byte. init() fills the tables
once, without keeping their contents as fixed maximum-sized initializers. Application VALUE storage,
TLS queues, native UART objects and rings, and the stack are additional.

## Examples

Examples 01..18 cover TLS scheduling. NLS examples continue the numbering:

| Example | Purpose |
|---|---|
| `19_NLS_LED_PingPong` | Same sketch on both MCUs; pass the LED light with a 1000 ms hold |
| `20_NLS_LED_Tempo` | Same sketch on both MCUs; each visits 1024, 960, ... 64, ... 1024 ms |
| `21_NLS_LED_Console` | Receive LED state and print it on the console |
| `22_NLS_LED_Remote` | Blink locally and commit each LED state change |

## Documentation

- [Shared API](docs/API.md)
- [Configuration](docs/CONFIGURATION.md)
- [Platforms and UART wiring](docs/PLATFORMS.md)
- [Protocol](docs/PROTOCOL.md)
- [TinyLoopScheduler README](https://github.com/MyTechWaifu/MTW_TinyLoopScheduler)
- [TinyLoopScheduler documentation](https://github.com/MyTechWaifu/MTW_TinyLoopScheduler/tree/main/docs)

## Citation

If you use this software, please cite MTW NetLoopScheduler using the metadata
in [CITATION.cff](CITATION.cff).

Project DOI, covering all versions:
[10.5281/zenodo.24027638](https://doi.org/10.5281/zenodo.24027638).

## License

Apache-2.0. See [LICENSE](LICENSE) and [NOTICE](NOTICE).
