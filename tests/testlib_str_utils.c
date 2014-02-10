
#include "../src/utils/yotta_str_utils.h"
#include <mk_test.h>

void
test_str_dec_to_ui64()
{
    uint64_t output = 0;

    test_assert(yotta_str_dec_to_ui64(&output, "") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "0a") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "01") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "hello") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "+1") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "-1") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "0x1") == -1ull);
    test_assert(yotta_str_dec_to_ui64(&output, "18446744073709551616") == -1ull);

    test_assert(yotta_str_dec_to_ui64(&output, "0") == 0 && output == 0);
    test_assert(yotta_str_dec_to_ui64(&output, "1") == 0 && output == 1);
    test_assert(yotta_str_dec_to_ui64(&output, "130") == 0 && output == 130);
    test_assert(
        yotta_str_dec_to_ui64(&output, "18446744073709551615") == 0 &&
        output == 18446744073709551615ull
    );
}

int
main()
{
    test_str_dec_to_ui64();
    return 0;
}

