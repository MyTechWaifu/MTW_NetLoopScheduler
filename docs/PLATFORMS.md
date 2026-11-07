# MTW NetLoopScheduler Platforms

## Supported target model

NLS is intended for AVR microcontrollers using Arduino-compatible cores and a
native hardware Serial object. Classic AVR, megaAVR 0-series and AVR Dx use the
same network algorithm and baud coefficient. Sufficient Flash/SRAM and native
RX/TX rings of at least 64 bytes each are required.

The library does not replace HardwareSerial/UARTClass or modify the core ISR.
Other AVR chips can be used when their core meets these requirements. TLS
support for a non-AVR architecture does not imply an implemented NLS backend.

Hardware-tested reference MCUs: ATmega2560, ATmega328PB, AVR128DB32.

## UART selection

Default NET is Serial1, leaving hardware UART0 for an optional application
console. Only the selected NET object is initialized. With one hardware UART,
define NLS_SERIAL as Serial and dedicate it to NET. On USB-native boards the
console may instead be USB Serial; consult the selected core's naming.

| Target/core | NET object | TX / RX |
|---|---|---|
| ATmega328PB / MiniCore | Serial1 | D11 / D12 |
| Mega2560 / Arduino AVR | Serial1 | D18 / D19 |
| Leonardo / Arduino AVR | Serial1 | D1 / D0; Serial is the USB console |
| Uno328P / Arduino AVR | Serial, explicit override | D1 / D0; no UART console |
| AVR DB32 / DxCore, USART2 alternate route | Serial2, swap(1) | PF4 / PF5; D24 / D25 on standard 32-pin variant |

Connect TX↔RX, RX↔TX and common GND, using compatible I/O levels. No D4/D5
handshake wiring is required. Do not connect multiple active TX outputs to one
RX input. Check any USB-UART connection on single-UART boards.

For the DB alternate route, select NLS_SERIAL=Serial2 before the include and
call Serial2.swap(1) in setup before tinyls::init(). The application may use
USART0 PA0/PA1 for its console. This route is specific to DB, not DD.
