
#include <mk_test.h>
#include "../src/utils/yotta_math.h"


static
void
test_is_pow2()
{
    test_assert(yotta_math_is_pow2(0) == 1); // limit case

    test_assert(yotta_math_is_pow2(1) == 1);
    test_assert(yotta_math_is_pow2(2) == 1);
    test_assert(yotta_math_is_pow2(3) == 0);
    test_assert(yotta_math_is_pow2(4) == 1);
    test_assert(yotta_math_is_pow2(1024) == 1);
    test_assert(yotta_math_is_pow2(~0u) == 0);
    test_assert(yotta_math_is_pow2(~0ull) == 0);
}

static
void
test_ceil_pow2_ui64()
{
    test_assert(yotta_math_ceil_pow2_ui64(0) == 0); // limit case
    test_assert(yotta_math_ceil_pow2_ui64(1) == 1);
    test_assert(yotta_math_ceil_pow2_ui64(2) == 2);
    test_assert(yotta_math_ceil_pow2_ui64(3) == 4);
    test_assert(yotta_math_ceil_pow2_ui64(24) == 32);
    test_assert(yotta_math_ceil_pow2_ui64(~0ull) == 0); // limit case
}

int
main()
{
    test_is_pow2();
    test_ceil_pow2_ui64();
    return 0;
}

