#ifndef _YOTTA_MATH
#define _YOTTA_MATH

#include "../yotta_prefix.h"


/*
 * @infos: Checks if a power of two
 *
 * @param <x> is the number to check
 *
 * @caution 0 will succeed the condition
 */
#define yotta_math_is_pow2(x) \
    ((((x) - 1) & (x)) == 0)


/*
 * @infos: Rounds <x> upward to a power of two
 *
 * @param <x>: is the value to round up to
 *
 * @returns: The power of two
 *
 * @caution x=0 will returns 0
 */
uint64_t
yotta_math_ceil_pow2_ui64(uint64_t x);

uint32_t
yotta_math_ceil_pow2_ui32(uint32_t x);


#endif
