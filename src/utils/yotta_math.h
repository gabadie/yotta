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


#endif
