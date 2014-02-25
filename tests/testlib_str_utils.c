
#include "../src/utils/yotta_str_utils.h"
#include <mk_test.h>
#include <string.h>

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

void
test_ui16_to_str()
{
    char str[6]; // Max: "65535\0" -> 6 chars

    test_assert(yotta_ui16_to_str(str, 0) == 0 && strcmp(str, "0") == 0);
    test_assert(yotta_ui16_to_str(str, 42) == 0 && strcmp(str, "42") == 0);
    test_assert(yotta_ui16_to_str(str, 65535) == 0 && strcmp(str, "65535") == 0);
}

int
main()
{
    test_str_dec_to_ui64();
    test_ui16_to_str();
    return 0;
}

