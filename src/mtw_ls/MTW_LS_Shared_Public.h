// SPDX-License-Identifier: Apache-2.0
#pragma once
/*
 Shared storage consists of ordinary mutable scalar variables. A direct
 assignment remains an ordinary assignment; commit() marks its current value
 for the nearest available NET phase. set() combines assignment and commit.
 No proxy classes, heap allocation or operator interception are involved.

 Calls belong to the main context, including TLS callbacks. Do not modify
 shared scalars or call these functions from a hardware ISR. In particular,
 multi-byte storage is not made atomic against application ISR writers.
*/
extern "C" uint8_t nls_shared_lookup(const void *, uint8_t) __attribute__((weak));

namespace netls {
namespace kernel {
enum Flag : uint8_t {
    HAS_DIRTY = 1,
    ONLINE = 2,
    HELLO_DONE = 4,
    PEER_SEEN = 8,
    BOOT_DONE = 16,
};
extern uint8_t flags;
extern "C" uint8_t nls_shared_bit_masks[] __attribute__((weak));
extern "C" uint8_t nls_shared_dirty[] __attribute__((weak));
} // namespace kernel
} // namespace netls

namespace shared {
namespace detail {

// Setup validation is evaluated at compile time. This address list is
// never used at run time and does not remain as a second RAM table.
constexpr bool uniqueRow(const void *const *p, unsigned row, unsigned j)
{
    return j == row || (p[row] != p[j] && uniqueRow(p, row, j + 1u));
}
constexpr bool uniqueAll(const void *const *p, unsigned n, unsigned row)
{
    return row == n || (uniqueRow(p, row, 0u) && uniqueAll(p, n, row + 1u));
}
} // namespace detail

/*
 Concrete scalar overloads keep the shared API procedural. Each overload uses
 the native C++ assignment conversion for its destination type. No type-erased
 VALUE copy or per-variable wrapper is needed. Short paths are available for
 compiler inlining; the generated address lookup also handles run-time references.

 commit() marks existing storage. set() assigns only registered storage, then
 marks it. Both return false for unknown storage, without changing VALUE.
 Calls belong to main context. Repeated commits coalesce into one dirty bit.
*/

inline bool commit(bool &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(bool &value, bool next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(char &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(char &value, char next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(signed char &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(signed char &value, signed char next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(unsigned char &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(unsigned char &value, unsigned char next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(short &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(short &value, short next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(unsigned short &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(unsigned short &value, unsigned short next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(int &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(int &value, int next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(unsigned int &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(unsigned int &value, unsigned int next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(long &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(long &value, long next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(unsigned long &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(unsigned long &value, unsigned long next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(long long &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(long long &value, long long next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(unsigned long long &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(unsigned long long &value, unsigned long long next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(float &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(float &value, float next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(double &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(double &value, double next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool commit(long double &value)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

inline bool set(long double &value, long double next)
{
    if (nls_shared_lookup == 0) {
        return false;
    }

    const uint8_t index = nls_shared_lookup(&value, (uint8_t)sizeof(value));
    if (index == 255u) {
        return false;
    }

    value = next;
    netls::kernel::nls_shared_dirty[index >> 3] |= (uint8_t)(1u << (index & 7u));
    netls::kernel::flags |= netls::kernel::HAS_DIRTY;
    return true;
}

} // namespace shared

/* One SHARED_CONFIG at file scope, in the implementation-owning translation
   unit. It may be global or inside a namespace. Each tuple is (name),
   (name, type), or (name, type, initial_value); forms may be mixed. */
#include "MTW_LS_Shared_Macros.h"
