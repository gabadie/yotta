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
#include <stdio.h>
#include <inttypes.h>

#include "yotta_dictate_daemon_info.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"

void
yotta_dictate_daemon_info_recv(
    yotta_dictate_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(cmd_queue->vtable != NULL);

    // Daemon info struct
    typedef struct
    yotta_dictate_buffer_s
    {
        uint64_t data_cursor;
        struct
        {
            uint64_t nb_computers;
            uint64_t nb_threads;
        }
        data;
    }
    yotta_dictate_buffer_t;

    yotta_dictate_buffer_t * buffer =
        yotta_dictate_queue_recv_buffer(cmd_queue, yotta_dictate_buffer_t);

    // If we have not received all the daemon info yet..
    if (buffer->data_cursor < sizeof(buffer->data))
    {
        // ..we try to get it
        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->data),
            &buffer->data_cursor,
            &buffer->data
        );

        if (op != 0) // We did not receive all the data
        {
            return;
        }
    }

    // Call the vtable's function linked to the daemon info reception
    cmd_queue->vtable->receive_daemon_info(cmd_queue, buffer->data.nb_computers, buffer->data.nb_threads);

    // we clean up the tmp buffer
    buffer->data_cursor = 0;
    buffer->data.nb_computers = 0;
    buffer->data.nb_threads = 0;

    yotta_dictate_queue_finish(cmd_queue);
}

void
yotta_dictate_vtable_daemon_info_recv(yotta_dictate_queue_t * queue, uint64_t nb_computers, uint64_t nb_threads)
{
    (void) queue;
    fprintf(stderr, "Nb computers: %" PRIu64 " | nb threads: %" PRIu64 "\n", nb_computers, nb_threads);
}
