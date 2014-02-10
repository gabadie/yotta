
#include "../src/utils/yotta_str_utils.h"
#include <mk_test.h>

void
test_str_dec_to_ui64()
{
    uint64_t output = 0;

    test_assert("test parsing uint64_t \"\"", yotta_str_dec_to_ui64(&output, "") == -1ull);
    test_assert("test parsing uint64_t \"0a\"", yotta_str_dec_to_ui64(&output, "0a") == -1ull);
    test_assert("test parsing uint64_t \"01\"", yotta_str_dec_to_ui64(&output, "01") == -1ull);
    test_assert("test parsing uint64_t \"hello\"", yotta_str_dec_to_ui64(&output, "hello") == -1ull);
    test_assert("test parsing uint64_t \"+1\"", yotta_str_dec_to_ui64(&output, "+1") == -1ull);
    test_assert("test parsing uint64_t \"-1\"", yotta_str_dec_to_ui64(&output, "-1") == -1ull);
    test_assert("test parsing uint64_t \"0x1\"", yotta_str_dec_to_ui64(&output, "0x1") == -1ull);
    test_assert("test parsing uint64_t \"18446744073709551616\"",
        yotta_str_dec_to_ui64(&output, "18446744073709551616") == (uint64_t)-1
    );

    test_assert("test parsing uint64_t \"0\"", yotta_str_dec_to_ui64(&output, "0") == 0 && output == 0);
    test_assert("test parsing uint64_t \"1\"", yotta_str_dec_to_ui64(&output, "1") == 0 && output == 1);
    test_assert("test parsing uint64_t \"130\"", yotta_str_dec_to_ui64(&output, "130") == 0 && output == 130);
    test_assert("test parsing uint64_t \"18446744073709551615\"",
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

