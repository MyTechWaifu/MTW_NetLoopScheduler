// SPDX-License-Identifier: Apache-2.0
/*
  MTW NetLoopScheduler — LED Console

  Upload to the board connected to the PC. Pair with 22_NLS_LED_Remote.
  Open Serial Monitor at 115200 baud. NET uses Serial1; console uses Serial.
  Cross the NET TX/RX wires and connect GND. Both tables have one bool row.

  poll() reads an ordinary variable. A valid incoming NET record updates it
  automatically. The console prints only when that value changes.
*/
#include <MTW_NetLoopScheduler.h>

// Both sketches register one bool in the same table position. NET knows its
// byte size from this table; it does not transmit a type name or a length.
SHARED_CONFIG(
    (led, bool, false)
);

// Local comparison copy. It is not shared and creates no outgoing traffic.
static bool previous = false;

static void showChange() {
    // Nothing changed: return without printing on this scheduler pass.
    if (led == previous) {
        return;
    }
    // Store the new value so the next POLL does not print it again.
    previous = led;
    if (led) {
        Serial.println(F("LED ON"));
    } else {
        Serial.println(F("LED OFF"));
    }
}

void setup() {
    Serial.begin(115200);
    // Do not wait for a PC connection: the scheduler also runs autonomously.
    // Start the scheduler and NET port. Only the Console sketch starts Serial.
    tinyls::init();
    tinyls::poll(showChange);
}

void loop() {
    // Keep calling tick; UART ISR transmission/reception proceeds in parallel.
    tinyls::tick();
}
