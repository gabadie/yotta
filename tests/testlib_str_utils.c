
#include "../src/utils/yotta_str_utils.h"
#include <mk_test.h>
#include <string.h>

void
test_str_dec_to_ui64()
{
    uint64_t output = 0;

    test_assert(yotta_str_dec_to_ui64(&output, "") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "0a") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "01") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "hello") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "+1") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "-1") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "0x1") != 0);
    test_assert(yotta_str_dec_to_ui64(&output, "18446744073709551616") != 0);

    test_assert(yotta_str_dec_to_ui64(&output, "0") == 0 && output == 0);
    test_assert(yotta_str_dec_to_ui64(&output, "1") == 0 && output == 1);
    test_assert(yotta_str_dec_to_ui64(&output, "130") == 0 && output == 130);
    test_assert(
        yotta_str_dec_to_ui64(&output, "18446744073709551615") == 0 &&
        output == 18446744073709551615ull
    );
}

void
testhelper_ui64_to_str(char const * string, uint64_t number, uint64_t basis)
{
    char result[32];

    yotta_ui64_to_str(result, number, basis);

    test_assert(strcmp(string, result) == 0);
}

void
test_ui64_to_str()
{
    testhelper_ui64_to_str("0", 0, 2);
    testhelper_ui64_to_str("0", 0, 8);
    testhelper_ui64_to_str("0", 0, 10);
    testhelper_ui64_to_str("0", 0, 16);

    testhelper_ui64_to_str("7", 7, 10);
    testhelper_ui64_to_str("10", 10, 10);
    testhelper_ui64_to_str("436", 436, 10);

    testhelper_ui64_to_str("7", 07, 8);
    testhelper_ui64_to_str("10", 010, 8);
    testhelper_ui64_to_str("436", 0436, 8);

    testhelper_ui64_to_str("7", 0x7, 16);
    testhelper_ui64_to_str("10", 0x10, 16);
    testhelper_ui64_to_str("436", 0x436, 16);
}

int
main()
{
    test_str_dec_to_ui64();
    test_ui64_to_str();
    return 0;
}

