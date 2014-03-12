
#include "../src/utils/yotta_str_utils.h"
#include <mk_test.h>
#include <string.h>


static
void
test_str_to_ui64_null()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 2) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 8) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 10) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 16) == 0 && output == 0);
}

static
void
test_str_to_ui64_dec()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "0a", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "01", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "hello", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "+1", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "-1", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "0x1", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "18446744073709551616", 10) != 0);

    test_assert(yotta_str_to_ui64(&output, "0", 10) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "00", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "1", 10) == 0 && output == 1);
    test_assert(yotta_str_to_ui64(&output, "130", 10) == 0 && output == 130);
    test_assert(
        yotta_str_to_ui64(&output, "18446744073709551615", 10) == 0 &&
        output == 18446744073709551615ull
    );
}

static
void
test_str_to_ui64_hex()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "g", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "0g", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "01", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "hello", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "+1", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "-1", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "0x1", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "10000000000000000", 16) != 0);

    test_assert(yotta_str_to_ui64(&output, "0", 16) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "00", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "1", 16) == 0 && output == 1);
    test_assert(yotta_str_to_ui64(&output, "fF", 16) == 0 && output == 0xfF);
    test_assert(yotta_str_to_ui64(&output, "1abcdef", 16) == 0 && output == 0x1abcdef);
    test_assert(yotta_str_to_ui64(&output, "130", 16) == 0 && output == 0x130);
    test_assert(
        yotta_str_to_ui64(&output, "FFFFFFFFFFFFFFFF", 16) == 0 &&
        output == 0xFFFFFFFFFFFFFFFFull
    );
}

static
void
test_str_to_ui64_detect()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0b0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "00", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0x0", 0) == 0 && output == 0);

    test_assert(yotta_str_to_ui64(&output, "0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0b10", 0) == 0 && output == 0x2);
    test_assert(yotta_str_to_ui64(&output, "042", 0) == 0 && output == 042);
    test_assert(yotta_str_to_ui64(&output, "42", 0) == 0 && output == 42);
    test_assert(yotta_str_to_ui64(&output, "0x42", 0) == 0 && output == 0x42);
}

static
void
testhelper_ui64_to_str(char const * string, uint64_t number, uint64_t basis)
{
    char result[32];

    yotta_ui64_to_str(result, number, basis);

    test_assert(strcmp(string, result) == 0);
}

static
void
test_ui64_to_str()
{
    testhelper_ui64_to_str("0", 0, 2);
    testhelper_ui64_to_str("0", 0, 8);
    testhelper_ui64_to_str("0", 0, 10);
    testhelper_ui64_to_str("0", 0, 16);

    testhelper_ui64_to_str("1", 0x1, 2);
    testhelper_ui64_to_str("10", 0x2, 2);

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
    test_str_to_ui64_null();
    test_str_to_ui64_dec();
    test_str_to_ui64_hex();
    test_str_to_ui64_detect();
    test_ui64_to_str();
    return 0;
}

