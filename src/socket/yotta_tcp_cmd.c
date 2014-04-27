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

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "yotta_tcp.h"
#include "yotta_tcp_cmd.private.h"
#include "yotta_tcp_queue.private.h"
#include "../core/yotta_debug.h"


uint64_t
yotta_tcp_cmd_send(yotta_tcp_cmd_t * cmd, uint64_t buffer_size, uint64_t * buffer_cursor, void const * buffer)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->queue != 0);
    yotta_assert(buffer_size != 0);
    yotta_assert(buffer_cursor != 0);
    yotta_assert(buffer != 0);

    // ilegal to send data without a lost connection callback
    yotta_assert(cmd->queue->socket_event.lost_event != 0);

    yotta_tcp_queue_t * queue = cmd->queue;

    ssize_t buffer_sending = buffer_size - *buffer_cursor;

    ssize_t buffer_sent = yotta_tcp_send_f(
        (yotta_socket_t *) queue,
        ((uint8_t *) buffer) + *buffer_cursor,
        buffer_sending,
        YOTTA_TCP_NOWAIT | YOTTA_TCP_NOSIGNAL
    );

    if (buffer_sent != -1)
    {
        /*
         * We sent something
         */

        *buffer_cursor += (uint64_t)buffer_sent;

        yotta_assert(*buffer_cursor <= buffer_size);

        return *buffer_cursor != buffer_size;
    }

    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
        /*
         * The send buffer is full then we tell the command's send callback to return
         */
        return 1;
    }

    /*
     * Something wrong appent, then we trigger the connection lost event
     */
    yotta_socket_event_lost(queue);

    /*
     * The connection lost event might have killed the queue and its commands.
     * Therefore we tell the command's send callback to return
     */
    return 1;
}

void
yotta_tcp_cmd_finish(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->queue != 0);
    yotta_assert(cmd->queue->queue_first == cmd);

    do // atomic command dequeuing
    {
        if (
            __sync_bool_compare_and_swap(
                &cmd->queue->queue_first,
                cmd,
                cmd->queue_next
            )
        )
        {
            break;
        }
    }
    while (1);

#ifdef YOTTA_ASSERT
    cmd->queue = NULL;
#endif //YOTTA_ASSERT

    yotta_assert(cmd->queue == 0);
}

#ifdef YOTTA_ASSERT

void
yotta_tcp_cmd_destroy(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    /*
     * make sure that the command has been removed from a TCP queue
     */
    yotta_assert(cmd->queue == NULL);
}

#endif
