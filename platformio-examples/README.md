# MTW NetLoopScheduler — PlatformIO Examples

Ready-to-build projects for
[MTW NetLoopScheduler](https://github.com/MyTechWaifu/MTW_NetLoopScheduler).
Each directory is an independent PlatformIO project:

```text
ExampleName/
  platformio.ini
  src/main.cpp
```

## Requirements

Use Visual Studio Code with PlatformIO IDE or PlatformIO Core CLI. The
project installs the library from GitHub:

```ini
[env]
platform = atmelavr
framework = arduino
lib_deps =
    https://github.com/MyTechWaifu/MTW_NetLoopScheduler.git
```

## Open and build

Open the individual example directory containing platformio.ini, not the
whole platformio-examples directory.

```bash
pio run -e megaatmega2560
pio run -e leonardo
pio run -e atmega328pb
```

Mega2560 is the default environment. ATmega328PB selects MiniCore; it is not
the single-UART ATmega328P. Board/core package availability is provided by
PlatformIO, not by the NLS library.

## Upload and monitor

```bash
pio run -e megaatmega2560 -t upload --upload-port COM5
pio device monitor --port COM5 --baud 115200
```

Select the actual board, programmer/bootloader and port. Monitor speed must
match the example's console baud; most legacy TLS console examples use 9600 baud,
whereas the NLS console example uses 115200 baud. Examples without a console produce
no application monitor output.

## NLS examples

19_NLS_LED_PingPong and 20_NLS_LED_Tempo use the same project on both boards.
21_NLS_LED_Console runs on the console board; 22_NLS_LED_Remote runs on its peer.
Use crossed TX/RX and common GND. No additional handshake pins are needed.

Only 19, 20, and 22 offer uno_net, where the sole Serial port is dedicated to NET.
The console example requires a separate console interface. Legacy examples 01..18 keep
their TLS task behavior while the NLS entry point also services the NET UART.

For different CPU clocks, set a common NLS_BAUD on both ends. Configuration
shared by translation units belongs in build_flags. See
[configuration](../docs/CONFIGURATION.md) and [platforms](../docs/PLATFORMS.md).
