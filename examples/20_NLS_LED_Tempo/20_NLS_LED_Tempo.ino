// SPDX-License-Identifier: Apache-2.0
/*
  MTW NetLoopScheduler — LED Tempo

  Upload the SAME sketch to both boards. Cross NET TX/RX and connect GND.
  Nano 328PB / MiniCore: Serial1 TX=D11, RX=D12. No console.

  A one-shot AFTER uses netls::random() for a 0..255 ms startup delay.
  After that delay, this board publishes a startup value with RUN cleared.
  POLL keeps changing the low seven bits until it observes a different value
  received from the peer. All local writes update previous, so they do not
  start the LED themselves. The first recipient publishes RUN and lights up;
  the peer receives RUN and waits for a changed TURN bit.

  If one board restarts, it publishes startup again. The running peer cancels
  its old LED-off task, turns its LED off and starts a fresh game at RUN.
  A stale RUN received before this board's initial AFTER is ignored: we must
  announce our own startup first. Tempo restarts its duration sequence at
  1024 ms. Startup commits can coalesce; they are not a queue of events.

  Both boards use the same sketch, with one ordinary shared byte. No node ID,
  private protocol state, receive callback or application random generator is
  used. NLS owns HELLO, ASK arbitration, acknowledgements and retries. These
  examples still rely on NLS making progress; they cannot resolve a protocol
  collision with perfectly identical timing on both boards.
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
static bool initialized = false;   // The current game has started.
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
    // A POST queued before restart must not light the LED during startup.
    if (!initialized || !(control & RUN)) {
        return;
    }
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

// Announce OUR startup before accepting an old working value from the peer.
// This AFTER runs only once. The zero has RUN cleared and requests a new game.
static void beginStartup()
{
    starting = true;
    shared::set(control, 0);
    previous = control;
}

// POLL visits this on every scheduler pass. It only posts LED work when a
// relevant received change is observed; it does not repeatedly create AFTERs.
static void watchControl()
{
    // The initial AFTER must publish startup first. Merely receiving an old
    // RUN is not enough: otherwise both boards could end up waiting for a turn.
    if (!starting) {
        return;
    }

    // A restarted peer sends a value with RUN cleared. Cancel our old AFTER
    // and extinguish the LED before the same startup code below starts again.
    if (initialized && !(control & RUN)) {
        tinyls::removeAfter(lightOff);
        digitalWrite(LED_BUILTIN, LOW);
        initialized = false;
    }

    if (!initialized) {
        // The peer published RUN: startup is over; wait for its first handoff.
        if (control & RUN) {
            initialized = true;
            previous = control;
        } else if (control != previous) {
            // All our own writes update previous; this change came from NET.
            // A different incoming startup value selects us as the starter.
            // Clear old LED work and publish RUN with TURN/Tempo phase zero.
            initialized = true;
            tinyls::removeAfter(lightOff);
            digitalWrite(LED_BUILTIN, LOW);
            shared::set(control, RUN);
            previous = control;
            tinyls::post(lightOn);
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
