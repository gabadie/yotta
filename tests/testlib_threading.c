
#include <stdio.h>
#include <yotta.h>
#include <mk_test.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

void
test_threading_cores()
{
    uint64_t cores = 0;

    test_assert(yotta_threading_cores(&cores) == 0);
    test_assert(cores >  0);

    printf("number of cores: %" PRIu64 "\n", cores);
}

int
main()
{
    test_threading_cores();

    return 0;
}
