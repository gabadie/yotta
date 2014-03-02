
#include <stdio.h>
#include <yotta.h>
#include <mk_test.h>


void
test_threading_cores()
{
    uint64_t cores = 0;

    test_assert(yotta_threading_cores(&cores) == 0);
    test_assert(cores >  0);

    printf("number of cores: %llu\n", cores);
}

int
main()
{
    test_threading_cores();

    return 0;
}
