
//#include "testhelper_memory.h"
#include "testhelper_init.h"
#include "testhelper_lorem.h"


uint64_t validity_parameter;

static
void
test_validity_callback(uint64_t * param)
{
    test_assert(param == &validity_parameter)

    {
        uint64_t r = yotta_dispatch((void *) test_validity_callback, param, 0);

        test_assert(r == YOTTA_INVALID_OPERATION);
    }

    __sync_fetch_and_add(&validity_parameter, 1);
}

static
void
test_dispatch_validity()
{
    uint64_t cores = 0;

    validity_parameter = 0;

    test_assert(yotta_threading_online_cores(&cores) == YOTTA_SUCCESS);

    {
        uint64_t r = yotta_dispatch(0, 0, 0);

        test_assert(r == YOTTA_INVALID_VALUE);
    }

    {
        uint64_t r = yotta_dispatch((void *) test_validity_callback, &validity_parameter, 0);

        test_assert(r == YOTTA_SUCCESS);
    }

    test_assert(validity_parameter == cores);
}

static
void
test_default_id()
{
    uint64_t local_id = 0;
    uint64_t local_count = 0;
    uint64_t group_id = 0;
    uint64_t group_count = 0;
    uint64_t global_id = 0;
    uint64_t global_count = 0;

    yotta_get_local_id(&local_id, &local_count);
    yotta_get_group_id(&group_id, &group_count);
    yotta_get_global_id(&global_id, &global_count);

    test_assert(local_id == 0);
    test_assert(local_count == 1);
    test_assert(group_id == 0);
    test_assert(group_count == 1);
    test_assert(global_id == 0);
    test_assert(global_count == 1);
}

static
void
test_id_callback(uint64_t * output_array)
{
    uint64_t cores = 0;
    uint64_t local_id = 0;
    uint64_t local_count = 0;
    uint64_t group_id = 0;
    uint64_t group_count = 0;
    uint64_t global_id = 0;
    uint64_t global_count = 0;

    test_assert(output_array != 0);

    test_assert(yotta_threading_online_cores(&cores) == YOTTA_SUCCESS);

    yotta_get_local_id(&local_id, &local_count);
    yotta_get_group_id(&group_id, &group_count);
    yotta_get_global_id(&global_id, &global_count);

    output_array[local_id] = local_id;
    test_assert(local_count == cores);
    test_assert(group_id == 0);
    test_assert(group_count == 1);
    test_assert(global_id == local_id);
    test_assert(global_count == local_count);
}

static
void
test_dispatch_id()
{
    uint64_t cores;

    test_assert(yotta_threading_online_cores(&cores) == YOTTA_SUCCESS);

    uint64_t * output_array = yotta_alloc_sa(uint64_t, cores);

    testhelper_lorem(output_array, sizeof(*output_array) * cores);

    {
        uint64_t r = yotta_dispatch((void *) test_id_callback, output_array, 0);

        test_assert(r == YOTTA_SUCCESS);
    }

    for (uint64_t i = 0; i < cores; i++)
    {
        test_assert(output_array[i] == i);
    }

    yotta_free(output_array);
}

static
void
test_stride_callback(uint64_t * param)
{
    uint64_t local_id = 0;

    yotta_get_local_id(&local_id, 0);

    test_assert(param == (void *)(42 + local_id * 13));
}

static
void
test_dispatch_param_stride()
{
    {
        uint64_t r = yotta_dispatch((void *) test_stride_callback, (void *) 42, 13);

        test_assert(r == YOTTA_SUCCESS);
    }
}

int
main()
{
    testhelper_init();
    //testhelper_memory_setup();

    test_dispatch_validity();
    test_default_id();
    test_dispatch_id();
    test_dispatch_param_stride();

    return 0;
}
