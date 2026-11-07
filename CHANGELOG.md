# Changelog

## 0.1.0

Initial release.

- Lightweight cooperative scheduling based on MTW TinyLoopScheduler:
  PERIODIC, DELAYED, POSTED, POLL, deferred ISR work and optional PARTED tasks.
- Automatic shared-variable exchange between two AVR microcontrollers over
  one native hardware UART, with NET processing before scheduler IDLE.
- Ordinary numeric variables configured with one static SHARED_CONFIG table:
  initialized declarations, zero-initialized declarations and existing-variable
  bindings can be mixed in the same table, with up to 254 entries.
- Procedural shared::set() and shared::commit() APIs; pending changes are sent
  in the nearest available NET phase without a periodic autosync timer.
- Native core UART interrupts and ring buffers, raw VALUE bytes, mandatory
  VALUE-only SUM8, per-variable acknowledgements and retries.
- COLD, BROADCAST, RECOVER and BROADCAST_RECOVER startup policies, with HELLO
  handshake and timeout-based connection recovery.
- Configurable Serial object, common F_CPU/24 default baud coefficient and
  response timeout; UART0 remains available for an application console when
  another hardware UART is selected for NET.
- Public netls::random() application byte, also used for collision backoff;
  NLS_RANDOM selects COLLISION updates (default) or NETLESS updates on both
  collisions and completely empty NET passes.
- Static memory, no heap allocation, no custom UART driver, no function
  templates and no per-variable wrapper classes.
- Arduino examples for LED ping-pong, changing LED tempo and remote LED state
  reporting, with English API, configuration, platform and protocol documents.
