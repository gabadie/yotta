
#include "yotta_math.h"
#include "../core/yotta_debug.h"


uint64_t
yotta_math_ceil_pow2_ui64(uint64_t x)
{
    x -= 1;

    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    x |= (x >> 32);

    yotta_assert(yotta_math_is_pow2(x + 1));

    return x + 1;
}

uint32_t
yotta_math_ceil_pow2_ui32(uint32_t x)
{
#ifdef YOTTA_ASSERT
    uint32_t y = x;
#endif

    x -= 1;

    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);

    yotta_assert(x + 1 == (uint32_t) yotta_math_ceil_pow2_ui64((uint64_t) y));

    return x + 1;
}
