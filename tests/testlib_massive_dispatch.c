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

    test_assert(yotta_threading_cores(&cores) == YOTTA_SUCCESS);

    {
        uint64_t r = yotta_dispatch(0, 0, 0);

        test_assert(r == YOTTA_INVALID_VALUE);
    }

    {
        uint64_t r = yotta_dispatch((void *) test_validity_callback, &validity_parameter, 0);

        test_assert(r == YOTTA_SUCCESS);
    }

    test_assert(validity_parameter == cores);

    testhelper_memory_check();
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

    testhelper_memory_check();
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

    test_assert(yotta_threading_cores(&cores) == YOTTA_SUCCESS);

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

    test_assert(yotta_threading_cores(&cores) == YOTTA_SUCCESS);

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

    testhelper_memory_check();
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

    testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_dispatch_validity();
    test_default_id();
    test_dispatch_id();
    test_dispatch_param_stride();

    return 0;
}
