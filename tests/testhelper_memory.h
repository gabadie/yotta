#ifndef _HELPER_MEMORY
#define _HELPER_MEMORY

#include "testhelper_init.h"
#include "testhelper_lorem.h"


/*
 * @infos: count all allocation
 */
static
uint64_t testhelper_alloc_count = 0;

/*
 * @infos: free yotta_memory prefix (see yotta_memory_alloc_func_t)
 */
static
void
testhelper_memory_free(yotta_memory_prefix_t * memory)
{
    test_assert((uint64_t *) memory->user_data == &testhelper_alloc_count);
    test_assert(testhelper_alloc_count > 0);

    __sync_fetch_and_sub(&testhelper_alloc_count, 1);

    free(memory);
}

/*
 * @infos: allocs the memory (see yotta_memory_alloc_func_t)
 */
static
yotta_memory_prefix_t *
testhelper_memory_alloc(void * user_data, size_t size, size_t alignment)
{
    test_assert((uint64_t *) user_data == &testhelper_alloc_count);
    test_assert(size > 0);

    yotta_memory_prefix_t * memory = 0;

    test_assert(posix_memalign((void **) &memory, alignment, size + sizeof(yotta_memory_prefix_t)) == 0);
    test_assert(memory != 0);

    memory->free_function = testhelper_memory_free;
    memory->user_data = user_data;
    memory->size = size;
    memory->coins = 1;

    __sync_fetch_and_add(&testhelper_alloc_count, 1);

    // put some dirty content before to make sure there is no forgotten initialization
    testhelper_lorem(memory + 1, size);

    return memory;
}

/*
 * @infos: sets up the memory tracker
 */
static
void
testhelper_memory_setup()
{
    test_assert(yotta_memory_set_allocator(testhelper_memory_alloc, &testhelper_alloc_count) == YOTTA_SUCCESS);
}

/*
 * @infos: get the number of allocation
 */
#define testhelper_memory_tracker() \
    testhelper_alloc_count

/*
 * @infos: checks up if there is memory leak
 */
#define testhelper_memory_check() \
    test_assert2("memory leak detected", testhelper_memory_tracker() == 0)


#endif
