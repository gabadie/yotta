/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */

#include <stdlib.h>

#ifdef YOTTA_DEBUG_DIRTY
#include <string.h>
#endif

#include "yotta_memory.h"
#include "yotta_return.private.h"
#include "../utils/yotta_math.h"


static
void
yotta_memory_default_free(yotta_memory_prefix_t * memory)
{
    free(memory);
}

static
yotta_memory_prefix_t *
yotta_memory_default_alloc(void * user_data, size_t size, size_t alignment)
{
    yotta_memory_prefix_t * memory = 0;

    if (posix_memalign(
        (void **) &memory,
        alignment,
        size + sizeof(yotta_memory_prefix_t)
    ) != 0)
    {
        yotta_crash_msg("posix_memalign failed to allocate");
    }

    yotta_assert(memory != 0);

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
yotta_alloc(size_t size, size_t alignment)
{
    yotta_assert(size != 0);
    yotta_assert(yotta_memory_allocator != 0);
    yotta_assert(alignment >= YOTTA_DEFAULT_ALIGNMENT);
    yotta_assert(yotta_math_is_pow2(alignment)); // alignment must be a power of two

    yotta_memory_prefix_t * memory = yotta_memory_allocator(
        yotta_memory_user_data,
        size,
        alignment
    );

    yotta_assert(memory->free_function != 0);
    yotta_assert(memory->size == size);
    yotta_assert(memory->coins == 1);
    yotta_assert((((size_t) memory) & (alignment - 1)) == 0);

#ifdef YOTTA_DEBUG_DIRTY
    memset(memory + 1, 0xFE, size);
#endif

    return memory + 1;
}

void
yotta_free(void * ptr)
{
    yotta_assert(ptr != 0);
    yotta_assert((((size_t) ptr) & 0xF) == 0);

    yotta_memory_prefix_t * memory = yotta_memory_prefix(ptr);

    yotta_assert(memory->free_function != 0);
    yotta_assert(memory->size != 0);

    memory->free_function(memory);
}

yotta_return_t
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
