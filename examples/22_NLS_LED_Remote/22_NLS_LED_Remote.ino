// SPDX-License-Identifier: Apache-2.0
/*
  MTW NetLoopScheduler — LED Remote

  Upload to the second board and pair with 21_NLS_LED_Console. This sketch
  never starts the console. Serial1 is the default NET port.

  every() changes the LED once per second: 1000 ms on, 1000 ms off. set()
  assigns the bool and marks it for the nearest available NET phase. The PC
  board receives the value into its corresponding ordinary bool variable.
*/
#include <MTW_NetLoopScheduler.h>

// Both sketches register one bool in the same table position. NET knows its
// byte size from this table; it does not transmit a type name or a length.
SHARED_CONFIG(
    (led, bool, false)
);

static void toggleLED() {
    // set() writes the bool and marks it dirty. The next available NET phase
    // starts delivery; there is no periodic autosync timer to wait for.
    shared::set(led, !led);
    digitalWrite(LED_BUILTIN, led ? HIGH : LOW);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    // Start the scheduler and NET port. Only the Console sketch starts Serial.
    tinyls::init();
    // One callback every second toggles the state: one second ON, one OFF.
    tinyls::every(toggleLED, 1000u);
}

void loop() {
    // Keep calling tick; UART ISR transmission/reception proceeds in parallel.
    tinyls::tick();
}
