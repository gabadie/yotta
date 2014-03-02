
//#include "testhelper_memory.h"
#include "testhelper_init.h"
#include "testhelper_lorem.h"


/*
 * Tests a group barrier on a lonely thread (that has not been dispatched)
 */
static
void
test_group_barrier_lonely()
{
    yotta_group_barrier();

    test_passed("group barrier on a lonely thread is not blocking");
}

/*
 * Sees test_group_barrier()
 */
static
void
test_group_barrier_callback(uint64_t * output_array)
{
    test_assert(output_array != 0);

    uint64_t local_id = 0;

    yotta_get_local_id(&local_id, 0);

    output_array[local_id] = local_id;

    yotta_group_barrier();

    uint64_t sum = 0;

    for (uint64_t i = 0; i <= local_id; i++)
    {
        sum += output_array[i];
    }

    yotta_group_barrier();

    output_array[local_id] = sum;
}

/*
 * Tests a group barrier on dispatched threads
 */
static
void
test_group_barrier()
{
    uint64_t cores = 0;

    test_assert(yotta_threading_cores(&cores) == YOTTA_SUCCESS);

    uint64_t * output_array = yotta_alloc_sa(uint64_t, cores);

    testhelper_lorem(output_array, yotta_memory_size(output_array));

    {
        uint64_t r = yotta_dispatch((void *) test_group_barrier_callback, output_array, 0);

        test_assert(r == YOTTA_SUCCESS);
    }

    for (uint64_t i = 0; i < cores; i++)
    {
        test_assert(output_array[i] == ((i * (i + 1)) / 2));
    }

    yotta_free(output_array);
}


int
main()
{
    testhelper_init();
    //testhelper_memory_setup();

    test_group_barrier_lonely();
    test_group_barrier();

    return 0;
}
