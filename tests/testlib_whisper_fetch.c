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

#include <string.h>

#include "testhelper_memory.h"
#include "testhelper_whisper.h"
#include "../src/whisper/yotta_whisper_fetch.private.h"


static
void
test_whisper_fetch_stress()
{
    static uint64_t const data_size = 1024 * 1024;
    testhelper_whisper_protocol_t protocol;

    void * src_data = malloc(data_size);
    void * dest_data = malloc(data_size);

    testhelper_whisper_protocol_init(&protocol);
    testhelper_lorem(src_data, data_size);

    {
        yotta_sync_t sync_fetch;

        yotta_dirty_s(&sync_fetch);

        yotta_whisper_fetch(&protocol.queue1, (uint64_t) src_data, data_size, dest_data, &sync_fetch);
        yotta_sync_wait(&sync_fetch);
    }

    test_assert(memcmp(src_data, dest_data, data_size) == 0);

    free(src_data);
    free(dest_data);

    testhelper_whisper_protocol_destroy(&protocol);

    testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_whisper_fetch_stress();

    return 0;
}
