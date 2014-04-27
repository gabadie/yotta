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

#include "testhelper_memory.h"
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

    testhelper_memory_check();
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

    testhelper_memory_check();
}


int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_group_barrier_lonely();
    test_group_barrier();

    return 0;
}
