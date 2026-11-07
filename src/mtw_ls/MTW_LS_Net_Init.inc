// SPDX-License-Identifier: Apache-2.0
/* NET initialization is part of tinyls::init(), like the TLS initialization. */
{
    using namespace ::netls::kernel;

    // Called by tinyls::init(). Configuration already exists as static data.
    // Empty configuration is valid; undefined weak storage is never touched.
    // Only the selected native UART begins; the console UART is untouched.
    flags = 0;
    phase = IDLE;
    rxStage = HEADER;
#if TLS_USE_IDLE == TLS_ENABLE
    deadline16 = tinyls::kernel::idle_prev_ms;
#else
    deadline16 = (uint16_t)tinyls::now;
#endif
    cursor = 0;
    if ((&nls_shared_count ? nls_shared_count : 0u)) {
        // The same initialization and lookup algorithms serve every N.
        // Only array bounds/constants come from SHARED_CONFIG. Tables live
        // in BSS and are built once, including after a repeated init().
        const uint8_t count = nls_shared_count;
        const uint8_t masks = count < 8u ? count : 8u;
        uint8_t mask = 1;
        for (uint8_t i = 0; i < masks; ++i) {
            nls_shared_bit_masks[i] = mask;
            mask <<= 1;
        }
        const uint8_t patterns = (uint8_t)(1u << (count < 4u ? count : 4u));
        nls_shared_first_bit[0] = 0;
        for (uint8_t i = 1; i < patterns; ++i) {
            nls_shared_first_bit[i] = (i & 1u) ? 0u : (uint8_t)(nls_shared_first_bit[i >> 1] + 1u);
        }
        memset(nls_shared_dirty, 0,
               (uint8_t)(((&nls_shared_count ? nls_shared_count : 0u) + 7u) / 8u));
    }
    helloWanted = RESET;
    NLS_SERIAL.begin(NLS_BAUD);
    // Sample once at the end of NET init through the native Arduino API.
    // Drop two bits: classic 16 MHz AVR micros() advances in 4 us steps.
    // The same expression serves every core; no timer registers are touched.
    // Original micros() bit 2 becomes bit 0 after the shift. If it is zero,
    // invert the seed. No further micros() samples are used by random mixing.
    randomByte = (uint8_t)(micros() >> 2);
    if (!(randomByte & 1u))
        randomByte = (uint8_t)~randomByte;
}
