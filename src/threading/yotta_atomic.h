#ifndef _YOTTA_ATOMIC
#define _YOTTA_ATOMIC

#include "../yotta_prefix.h"


#if defined(__GNUC__)


/*
 * @infos: defines atomic operations
 *
 * @doc: http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html
 */

#define yotta_atomic_fetch_add(ptr, value) \
    __sync_fetch_and_add(ptr, value)

#define yotta_atomic_fetch_sub(ptr, value) \
    __sync_fetch_and_sub(ptr, value)

#define yotta_atomic_fetch_or(ptr, value) \
    __sync_fetch_and_or(ptr, value)

#define yotta_atomic_fetch_and(ptr, value) \
    __sync_fetch_and_and(ptr, value)

#define yotta_atomic_fetch_xor(ptr, value) \
    __sync_fetch_and_xor(ptr, value)

#define yotta_atomic_fetch_nand(ptr, value) \
    __sync_fetch_and_nand(ptr, value)


#define yotta_atomic_add_fetch(ptr, value) \
    __sync_add_and_fetch(ptr, value)

#define yotta_atomic_sub_fetch(ptr, value) \
    __sync_sub_and_fetch(ptr, value)

#define yotta_atomic_or_fetch(ptr, value) \
    __sync_or_and_fetch(ptr, value)

#define yotta_atomic_and_fetch(ptr, value) \
    __sync_and_and_fetch(ptr, value)

#define yotta_atomic_xor_fetch(ptr, value) \
    __sync_xor_and_fetch(ptr, value)

#define yotta_atomic_nand_fetch(ptr, value) \
    __sync_nand_and_fetch(ptr, value)


#define yotta_atomic_compare_swap_b(ptr, oldval, newval) \
    __sync_bool_compare_and_swap(ptr, oldval, newval)

#define yotta_atomic_compare_swap(ptr, oldval, newval) \
    __sync_val_compare_and_swap(ptr, oldval, newval)

#else
#error "yotta_atomic.h not supported"

#endif


#endif //_YOTTA_ATOMIC
