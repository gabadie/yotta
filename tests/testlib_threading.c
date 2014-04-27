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

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

void
test_threading_cores()
{
    uint64_t cores = 0;
    uint64_t max_cores = 0;
    uint64_t online_cores = 0;

    test_assert(yotta_threading_cores(&cores) == 0);
    test_assert(cores >  0);

    test_assert(yotta_threading_max_cores(&max_cores) == 0);
    test_assert(max_cores >  0);

    test_assert(yotta_threading_online_cores(&online_cores) == 0);
    test_assert(online_cores >  0);

    printf("number of cores: %" PRIu64 "\n", cores);
    printf("number of max cores: %" PRIu64 "\n", max_cores);
    printf("number of online cores: %" PRIu64 "\n", online_cores);

    testhelper_memory_check();
}

int
main()
{
    testhelper_memory_setup();
    test_threading_cores();

    return 0;
}
