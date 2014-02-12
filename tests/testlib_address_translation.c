#include <yotta.h>
#include <mk_test.h>
#include <inttypes.h>

char
func_a()
{
    return 'a';
}

int
func_b(int m)
{
    return 1337 * m;
}

int
main(int argc, char const * const * argv)
{
    yotta_init(argc, argv);

    yotta_rel_addr_t rel_a = yotta_address_absolute_to_relative((uint64_t) &func_a);
    uint64_t abs_a = yotta_address_relative_to_absolute(rel_a);
    printf("func_a absolute address: %" PRIu64 "\n", abs_a);
    printf("func_a relative address: %" PRIu64 "\n", rel_a);

    test_assert2("Absolute address of a", abs_a == (uint64_t) &func_a);
    test_assert2("a call", ((char (*)()) abs_a)() == func_a());


    yotta_rel_addr_t rel_b = yotta_address_absolute_to_relative((uint64_t) &func_b);
    uint64_t abs_b = yotta_address_relative_to_absolute(rel_b);
    printf("func_b absolute address: %" PRIu64 "\n", abs_b);
    printf("func_b relative address: %" PRIu64 "\n", rel_b);

    test_assert2("Absolute address of b", abs_b == (uint64_t) &func_b);
    test_assert2("b call", ((int (*)(int)) abs_b)(42) == func_b(42));

    return 0;
}

