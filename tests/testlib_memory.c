
#include "testhelper_init.h"
#include "testhelper_memory.h"


int
main()
{
    {
        static const
        size_t hello_size = 16;

        void * hello = yotta_alloc_d(hello_size);

        test_assert(hello != 0);

        yotta_memory_prefix_t * memory = yotta_memory_prefix(hello);

        test_assert(memory != 0);
        test_assert(memory->free_function != 0);
        test_assert(memory->user_data == 0);
        test_assert(memory->size == hello_size);
        test_assert(memory->coins == 1);
        test_assert(yotta_memory_size(hello) == hello_size);
        test_assert(testhelper_memory_tracker() == 0);

        yotta_free(hello);

        test_assert(testhelper_memory_tracker() == 0);
    }

    testhelper_init();

    {
        static const
        size_t hello_size = 128;

        void * hello = yotta_alloc_d(hello_size);

        test_assert(hello != 0);

        yotta_memory_prefix_t * memory = yotta_memory_prefix(hello);

        test_assert(memory != 0);
        test_assert(memory->free_function != 0);
        test_assert(memory->user_data == 0);
        test_assert(memory->size == hello_size);
        test_assert(memory->coins == 1);
        test_assert(testhelper_memory_tracker() == 0);

        yotta_free(hello);

        test_assert(testhelper_memory_tracker() == 0);
    }

    testhelper_memory_setup();

    {
        static const
        size_t hello_size = 64;

        test_assert(testhelper_memory_tracker() == 0);

        void * hello = yotta_alloc_d(hello_size);

        test_assert(hello != 0);

        yotta_memory_prefix_t * memory = yotta_memory_prefix(hello);

        test_assert(memory != 0);
        test_assert(memory->free_function == testhelper_memory_free);
        test_assert(memory->user_data == &testhelper_alloc_count);
        test_assert(memory->size == hello_size);
        test_assert(memory->coins == 1);
        test_assert(testhelper_memory_tracker() == 1);

        yotta_free(hello);

        test_assert(testhelper_memory_tracker() == 0);
    }

    {
        test_assert(yotta_memory_set_allocator(0, 0) == YOTTA_INVALID_VALUE);
    }

    testhelper_memory_check();

    return 0;
}
