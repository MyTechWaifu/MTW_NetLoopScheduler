#include <Arduino.h>

// SPDX-License-Identifier: Apache-2.0
/*
  MTW NetLoopScheduler — LED Tempo

  Upload the SAME sketch to both boards. Cross NET TX/RX and connect GND.
  Nano 328PB / MiniCore: Serial1 TX=D11, RX=D12. No console.

  AFTER allows local startup sending and LED lighting after a 0..255 ms startup delay.
  Incoming startup values are processed immediately. POLL changes and commits a
  seven-bit value, remembering every local write in previous. A different
  received value can then be recognized without a library receive callback.
  The first recipient publishes RUN and starts its LED. Its peer receives
  RUN and waits for the handoff. NLS owns ASK, backoff and UART transmission.

  Startup can make several commits: set() marks the current value dirty;
  it does not queue every intermediate value. If a received startup value
  equals our last local value, POLL keeps changing it until a difference is
  observed. This is a startup demonstration, not application restart recovery.
  Start or reset both boards for a fresh game. No extra handshake wires,
  node numbers, protocol callbacks or receive-notification API are used.
*/
#include <MTW_NetLoopScheduler.h>

// Declare an ordinary byte and register it for NET. Use the same table on
// both boards. A received valid VALUE is assigned directly to this variable.
// Direct assignment stays local; set()/commit() request transmission.
SHARED_CONFIG(
    (control, uint8_t, 0)
);

// Bit 7: startup finished. Bit 6: handoff toggled at each LED-off.
// Before RUN, the low seven bits hold a changing startup value.
// In Tempo, the low five bits later hold the duration phase.
static const uint8_t RUN = 128u;
static const uint8_t TURN = 64u;

static bool starting = false;      // The startup delay has elapsed.
static bool initialized = false;   // The startup exchange is complete.
static bool first = false;         // This board starts the first pulse.
static uint8_t previous = 0;       // Updated after every local write.

// AFTER calls this once after the hold time. Turn the local LED off before
// passing the light to the peer. Updating previous prevents our own write
// from being mistaken for a received handoff by POLL.
static void lightOff()
{
    digitalWrite(LED_BUILTIN, LOW);
    control ^= TURN;
    // TURN returns to zero after both boards used this duration.
    // The low five bits then advance to the next phase, 0..29.
    if (!(control & TURN)) {
        uint8_t phase = (uint8_t)((control & 31u) + 1u);
        if (phase == 30u) {
            phase = 0;
        }
        control = (uint8_t)(RUN | phase);
    }

    shared::commit(control);
    previous = control;
}

// POST calls this in main context. AFTER schedules the later LED-off;
// it does not wait here, so loop(), NET and the other tasks keep running.
static void lightOn()
{
    digitalWrite(LED_BUILTIN, HIGH);
    // Each duration is used twice: 1024/1024, 960/960, ... 64/64,
    // then 128/128, ... 1024/1024. Only the phase is transmitted.
    const uint8_t phase = control & 31u;
    uint16_t holdTime;
    if (phase <= 15u) {
        holdTime = (uint16_t)(1024u - phase * 64u);
    } else {
        holdTime = (uint16_t)((phase - 14u) * 64u);
    }
    tinyls::after(lightOff, holdTime);
}

// This one-shot task permits local startup sending after a 0..255 ms startup delay.
// If an incoming startup value selected us earlier, start the first pulse
// now. The initial AFTER has finished, leaving its queue slot available.
static void beginStartup()
{
    starting = true;
    if (first) {
        tinyls::post(lightOn);
    }
}

// POLL visits this on every scheduler pass. It only posts LED work when a
// relevant received change is observed; it does not repeatedly create AFTERs.
static void watchControl()
{
    if (!initialized) {
        // The peer published RUN: startup is over; wait for its first handoff.
        if (control & RUN) {
            initialized = true;
            previous = control;
        } else if (control != previous) {
            // All our own writes update previous; this change came from NET.
            // Recognize a differing received startup value. Publish RUN,
            // stop startup writes and take the first LED pulse locally.
            initialized = true;
            first = true;
            shared::set(control, RUN);
            previous = control;
            if (starting) {
                tinyls::post(lightOn);
            }
        } else if (starting) {
            // Keep the startup value changing until a peer value is seen.
            // set() marks it dirty; NLS sends current values in its own turns.
            shared::set(control, (uint8_t)((control + 1u) & 127u));
            previous = control;
        }
        return;
    }

    // XOR identifies changed bits. Only a changed TURN starts a new pulse.
    // Receiving the same value again does not create another LED task.
    if ((control & RUN) && ((control ^ previous) & TURN)) {
        previous = control;
        tinyls::post(lightOn);
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Initialize TLS and the NET UART. No console Serial.begin() is needed.
    // Defaults: COLD startup policy and NET on Serial1.
    tinyls::init();
    tinyls::poll(watchControl);
    // Read the byte sampled by NET init. Both boards run the same sketch;
    // the initial AFTER uses 0..255 ms without blocking loop or NET.
    tinyls::after(beginStartup, netls::random());
}

void loop()
{
    // Service tasks and NET frequently. The native UART ISR moves bytes in
    // the background; this call does not wait for their physical transmission.
    tinyls::tick();
}
