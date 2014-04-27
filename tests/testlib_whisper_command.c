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
#include "../src/whisper/yotta_whisper_command.private.h"

static
uint64_t thread_callback_signature = 0;

static
void
test_thread_callback(uint64_t * param)
{
    test_assert(*param == 9);
    test_assert(thread_callback_signature == 0);
    test_assert(yotta_memory_size(param) == sizeof(*param));

    (*param)++;
    thread_callback_signature++;

    test_assert(*param == 10);
}

static
void
test_whisper_command_invocation()
{
    testhelper_whisper_protocol_t protocol;

    testhelper_whisper_protocol_init(&protocol);

    {
        uint64_t param = 9;

        yotta_sync_t sync_sent;
        yotta_sync_t sync_finished;

        yotta_dirty_s(&sync_sent);
        yotta_dirty_s(&sync_finished);

        yotta_whisper_command(
            &protocol.queue0,
            (yotta_whisper_command_entry_t) test_thread_callback,
            sizeof(param),
            &param,
            0,
            1,
            0,
            1,
            &sync_sent,
            &sync_finished
        );

        test_assert(param == 9);

        test_assert(yotta_sync_wait(&sync_sent) == YOTTA_SUCCESS);

        test_assert(param == 9);

        test_assert(yotta_sync_wait(&sync_finished) == YOTTA_SUCCESS);

        test_assert(param == 9);
        test_assert(thread_callback_signature == 1);
    }

    testhelper_whisper_protocol_destroy(&protocol);

    testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_whisper_command_invocation();

    return 0;
}
