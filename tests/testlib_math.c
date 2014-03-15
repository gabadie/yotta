
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

int
main()
{
    test_is_pow2();
    return 0;
}

