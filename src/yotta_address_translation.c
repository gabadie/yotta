#include "yotta_address_translation.h"

#include <assert.h>


static void yotta_base_function() { }

static volatile const uint64_t yotta_base_address = (uint64_t) &yotta_base_function;


uint64_t yotta_address_absolute_to_relative(uint64_t address)
{
    return (uint64_t) address - yotta_base_address;
}

uint64_t yotta_address_relative_to_absolute(uint64_t offset)
{
    return (uint64_t) yotta_base_address + offset;
}

#ifdef _YOTTA_ADDRESS_TRANSLATION_TESTS

#include <stdio.h>

char func_a() { return 'a'; }

int func_b(int m) { return 1337 * m; }


int main()
{
    printf("Yotta base address: %" PRIu64 "\n", yotta_base_address);


    uint64_t rel_a = yotta_address_absolute_to_relative((uint64_t) &func_a);
    uint64_t abs_a = yotta_address_relative_to_absolute(rel_a);
    printf("func_a absolute address: %" PRIu64 "\n", abs_a);
    printf("func_a relative address: %" PRIu64 "\n", rel_a);

    assert(abs_a == (uint64_t) &func_a);
    assert(((char (*)()) abs_a)() == func_a()); // FIXME: We need to know the type of the function...


    uint64_t rel_b = yotta_address_absolute_to_relative((uint64_t) &func_b);
    uint64_t abs_b = yotta_address_relative_to_absolute(rel_b);
    printf("func_b absolute address: %" PRIu64 "\n", abs_b);
    printf("func_b relative address: %" PRIu64 "\n", rel_b);

    assert(abs_b == (uint64_t) &func_b);
    assert(((int (*)(int)) abs_b)(42) == func_b(42));

    return 0;
};

#endif
