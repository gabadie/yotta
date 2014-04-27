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
