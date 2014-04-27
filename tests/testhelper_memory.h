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
#ifndef _HELPER_MEMORY
#define _HELPER_MEMORY

#include <yotta.h>
#include <mk_test.h>

#include "testhelper_lorem.h"
#include "../src/threading/yotta_semaphore_pool.private.h"


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
    yotta_sem_pool_flush();

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
    { \
        yotta_sem_pool_flush(); \
        test_assert2("memory leak detected", testhelper_memory_tracker() == 0); \
    }


#endif
