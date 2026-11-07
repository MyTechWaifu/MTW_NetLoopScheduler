# MTW NetLoopScheduler API

## Header and scheduler

Include `<MTW_NetLoopScheduler.h>`. Call `tinyls::init()` from setup and
`tinyls::tick()` frequently from the normal Arduino loop. Initialization starts
only the selected NET UART; console initialization belongs to the application.

Scheduling APIs (`every`, `after`, `post`, `poll`, removal operations, PARTED,
and `postISR`) follow [TinyLoopScheduler](https://github.com/MyTechWaifu/MTW_TinyLoopScheduler/blob/main/docs/API.md).
NET is a cooperative phase before IDLE. The core's UART ISR transfers bytes;
callbacks and NET parsing execute in main context.

## Shared configuration

Define exactly one SHARED_CONFIG at file scope in the implementation-owning
translation unit. Additional translation units define TLS_DECL_ONLY before
including the header. All translation units use matching feature settings.

```cpp
namespace sensors {
    uint32_t counter = 0;
}

SHARED_CONFIG(
    (temperature, uint16_t, 20),
    (enabled, bool),
    (sensors::counter)
);
```

`(name,type,value)` declares an initialized variable; `(name,type)` declares a
zero-initialized variable; `(name)` binds existing mutable storage. The table
may itself be declared in a namespace. Both ends use identical row order and
VALUE sizes, although initial values may differ. There are at most 254 entries,
with sizes 1,2,4,8 bytes. IDs are generated as zero-based table index + 1.
Duplicate storage, const storage, and unsupported sizes are compile-time errors.

The API uses ordinary overloads for built-in numeric types: bool, character,
integer and floating-point types, including Arduino fixed-width aliases.
There are no function templates or per-variable wrapper classes.

## shared::set(variable, value)

Assign using normal C++ conversion and mark dirty. Returns true for configured
storage. Unknown storage returns false and is not modified. Equal values are
also marked; the library does not compare old and new contents.

## shared::commit(variable)

Mark configured storage dirty without assigning it. Returns false for unknown
storage. A direct assignment without commit does not request transmission.

A commit is eligible in the nearest NET phase, subject to link readiness and
current exchange ownership. There is no periodic autosync interval. Commits
coalesce: the library synchronizes the current state rather than every change.
Dirty clears when the current VALUE is enqueued. A later commit survives while
OK is pending. RETRY rereads current storage, consumes that dirty bit, and does
not replay an old TX snapshot. Timeout restores the outstanding variable's dirty.

## Receiving values

Read ordinary variable storage. NET applies a complete scalar only after its
VALUE-only SUM8 matches; it does not apply partial or bad values. Received
updates do not mark local dirty. There is no takeUpdated API. Applications can
compare with a previous value in poll if they need change-triggered work.
Multiple variables are not one atomic transaction. Choose one application
writer per variable when conflicting writes would be ambiguous.

## Call context

set/commit and shared modifications are main-context operations, including
scheduler callbacks. Hardware ISR writes, another CPU core, concurrent threads,
and nested tick calls are unsupported. Raw memory representation is sent as-is;
matching AVR types and table layouts are required.

## Namespaces

| Namespace | Role |
|---|---|
| tinyls | Public scheduler API |
| tinyls::kernel | Scheduler implementation |
| shared | Public shared-variable API |
| shared::detail | Shared implementation helpers |
| netls | Public application random-byte API; future commands |
| netls::kernel | Protocol implementation and state |

Internal names are not public API. There are no public link-state/diagnostic
queries, force-commit/get operations, raw-block API, or command callbacks in this
version. MANY mode is reserved.

## LED example startup

Examples 19 and 20 run the same sketch on both boards. One shared byte holds
startup, handoff, and (for Tempo) phase information. A local comparison copy is
updated after every local write. Before RUN, a changing startup value is committed
until a differing peer value is observed. The first recipient publishes RUN
and starts the first LED pulse; the peer waits for the changed handoff bit.

The one-shot AFTER uses netls::random() for a 0..255 ms startup delay. It enables local startup sending. Incoming startup values
are observed immediately; LED lighting waits until that initial AFTER has run.
Several startup commits may coalesce before the protocol sends the current
value. ASK arbitration and retry remain entirely inside NLS. There is no
application protocol hook or receive-notification API. These examples show
paired startup and normal handoff; reset both boards to start a fresh game.
They do not implement application recovery after one board restarts mid-game.

## netls::random()

Return the current pseudorandom byte as `uint8_t` for application code. The
value can be used for startup delays, application choices or other small
pseudorandom selections. NLS also uses the same stored byte for collision
backoff.

Call after `tinyls::init()`. The result is a byte with numeric range 0..255.
Reading is passive: it does not sample time, change the stored byte, consume a
sequence entry or schedule any network work. Repeated reads may return the
same value, and samples are not guaranteed to be uniform or independent.

`NLS_RANDOM` selects when NLS updates the byte:

| Policy | Updates |
|---|---|
| `COLLISION` (default) | Crossed ASK only; no random-update work in empty NET passes |
| `NETLESS` | Crossed ASK and completely empty NET passes |

Define the desired policy before including the header. See
[random configuration](CONFIGURATION.md#random-byte-policy) for the exact
startup, empty-pass and collision transformations. Neither policy reads
`micros()` after NET initialization.

The getter has no side effects; the library owns all updates to the stored
byte. No additional public link-state query, receive notification or protocol
callback is introduced by this API.
