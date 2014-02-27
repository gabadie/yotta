
#include <stdlib.h>

#include "yotta_memory.h"
#include "yotta_return.private.h"


static
void
yotta_memory_default_free(yotta_memory_prefix_t * memory)
{
    free(memory);
}

static
yotta_memory_prefix_t *
yotta_memory_default_alloc(void * user_data, size_t size)
{
    yotta_memory_prefix_t * memory = (yotta_memory_prefix_t *) malloc(
        size + sizeof(yotta_memory_prefix_t)
    );

    memory->free_function = yotta_memory_default_free;
    memory->user_data = user_data;
    memory->size = size;
    memory->coins = 1;

    return memory;
}


static
yotta_memory_alloc_func_t yotta_memory_allocator = yotta_memory_default_alloc;

static
void * yotta_memory_user_data = 0;


void *
yotta_alloc(size_t size)
{
    yotta_assert(size != 0);
    yotta_assert(yotta_memory_allocator != 0);

    yotta_memory_prefix_t * memory = yotta_memory_allocator(yotta_memory_user_data, size);

    yotta_assert(memory->free_function != 0);
    yotta_assert(memory->size == size);
    yotta_assert(memory->coins == 1);

    return memory + 1;
}

void
yotta_free(void * ptr)
{
    yotta_assert(ptr != 0);

    yotta_memory_prefix_t * memory = yotta_memory_prefix(ptr);

    yotta_assert(memory->free_function != 0);
    yotta_assert(memory->size != 0);

    memory->free_function(memory);
}

uint64_t
yotta_memory_set_allocator(yotta_memory_alloc_func_t allocator, void * user_data)
{
    if (allocator == 0)
    {
        yotta_return_inv_value(yotta_memory_set_allocator, allocator);
    }

    yotta_memory_allocator = allocator;
    yotta_memory_user_data = user_data;

    return YOTTA_SUCCESS;
}
