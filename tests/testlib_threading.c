
#include "testhelper_memory.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

void
test_threading_cores()
{
    uint64_t cores = 0;
    uint64_t max_cores = 0;
    uint64_t online_cores = 0;

    test_assert(yotta_threading_cores(&cores) == 0);
    test_assert(cores >  0);

    test_assert(yotta_threading_max_cores(&max_cores) == 0);
    test_assert(max_cores >  0);

    test_assert(yotta_threading_online_cores(&online_cores) == 0);
    test_assert(online_cores >  0);

    printf("number of cores: %" PRIu64 "\n", cores);
    printf("number of max cores: %" PRIu64 "\n", max_cores);
    printf("number of online cores: %" PRIu64 "\n", online_cores);

    testhelper_memory_check();
}

int
main()
{
    testhelper_memory_setup();
    test_threading_cores();

    return 0;
}
