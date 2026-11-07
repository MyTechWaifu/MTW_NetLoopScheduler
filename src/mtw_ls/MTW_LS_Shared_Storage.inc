// SPDX-License-Identifier: Apache-2.0
/*
 A DUO descriptor stores only address and byte size. Its zero-based position
 determines the wire number (index + 1); neither IDs nor a second ID index are
 stored. The one SHARED_CONFIG definition owns the exact-sized arrays.

 Undefined weak declarations permit ordinary TLS examples without a shared
 configuration. Always check the count symbol before dereferencing it. With a
 visible configuration the compiler can fold this guard and the count.
*/
typedef struct {
    uint8_t *ptr;
    uint8_t size;
} Object;
extern "C" {
extern Object nls_shared_objects[] __attribute__((weak));
extern const uint8_t nls_shared_count __attribute__((weak));
extern uint8_t nls_shared_dirty[] __attribute__((weak));
extern uint8_t nls_shared_bit_masks[] __attribute__((weak));
extern uint8_t nls_shared_first_bit[] __attribute__((weak));
extern const uint8_t nls_shared_search_mask __attribute__((weak));
extern const uint8_t nls_shared_tail_mask __attribute__((weak));
}
