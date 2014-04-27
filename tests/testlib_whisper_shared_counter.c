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
#include "testhelper_whisper.h"
#include "../src/whisper/yotta_whisper_shared_counter.private.h"


static
void
test_whisper_shared_counter()
{
    uint64_t const range_size = 16;
    yotta_counter_t counter = 0;
    yotta_shared_counter_t shared;

    { // test initialization
        testhelper_lorem_s(&shared);

        shared.master_counter_addr = yotta_addr(&counter);
        shared.range_size = range_size;
        shared.range_status = 0;
        shared.stock_size = range_size;

        test_assert(shared.range_size == shared.stock_size);
        test_assert(shared.range_size > 0);
    }

    testhelper_whisper_protocol_t protocol;
    testhelper_whisper_protocol_init(&protocol);

    { // protocol life expectency
        yotta_whisper_shared_counter(&protocol.queue1, &shared);

        sleep(1);
    }

    testhelper_whisper_protocol_destroy(&protocol);

    { // test end checking
        test_assert(counter == range_size);
    }

    testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_whisper_shared_counter();

    return 0;
}
