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
#include <errno.h>

#include "yotta_whisper_queue.private.h"
#include "yotta_whisper_labels.private.h"
#include "yotta_whisper_command.private.h"
#include "yotta_whisper_fetch.private.h"
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_shared_counter.private.h"
#include "yotta_whisper_stop.private.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_debug.h"
#include "../socket/yotta_tcp.h"
#include "../socket/yotta_socket_thread.h"


/*
 * @infos: groups all whisper protocol labels' entries
 *
 * @important: must the order of yotta_whisper_label_t (yotta_whisper_labels.private.h)
 */
static
yotta_whisper_recv_t const
yotta_whisper_label_entries[YOTTA_WHISPER_LABELS_COUNT] =
{
    (yotta_whisper_recv_t) yotta_whisper_command_feedback_recv,
    (yotta_whisper_recv_t) yotta_whisper_command_order_recv,
    (yotta_whisper_recv_t) yotta_whisper_fetch_answer_recv,
    (yotta_whisper_recv_t) yotta_whisper_fetch_request_recv,
    (yotta_whisper_recv_t) yotta_whisper_push_master_recv,
    (yotta_whisper_recv_t) yotta_whisper_shared_counter_answer_recv,
    (yotta_whisper_recv_t) yotta_whisper_shared_counter_request_recv,
    (yotta_whisper_recv_t) yotta_whisper_stop_recv
};

#ifdef YOTTA_DEBUG

/*
 * @infos: verify that the receive buffer has been cleaned
 *
 * @param <cmd_queue>: the command queue to verify
 */
static
void
yotta_whisper_queue_assert_buffer(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    for (size_t i = 0; i < sizeof(cmd_queue->recv_buffer); i++)
    {
        yotta_assert(cmd_queue->recv_buffer[i] == 0);
    }
}

#endif //YOTTA_DEBUG


static
void
yotta_whisper_queue_recv(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    while (1)
    {
        if (cmd_queue->callback != 0)
        {
            cmd_queue->callback(cmd_queue);

            if (cmd_queue->callback != 0)
            {
                // the callback has not finished to receive all data
                break;
            }

#ifdef YOTTA_DEBUG
            yotta_whisper_queue_assert_buffer(cmd_queue);
#endif //YOTTA_DEBUG
        }

        uint64_t label_cursor = 0;
        yotta_whisper_label_t label = 0;

        uint64_t op = yotta_tcp_queue_recv(&cmd_queue->tcp_queue, sizeof(label), &label_cursor, &label);

        if (op != 0)
        {
            // no incomming information available
            return;
        }

        yotta_assert(label < YOTTA_WHISPER_LABELS_COUNT);
        yotta_assert(yotta_whisper_label_entries[label] != 0);

        cmd_queue->callback = yotta_whisper_label_entries[label];

        yotta_assert(cmd_queue->callback != 0);
    }
}

static
void
yotta_whisper_queue_except(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_logger_error("yotta_whisper_queue_except: received a TCP socket exception -> releasing");

    yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);

    yotta_socket_event_release(cmd_queue);
}

static
void
yotta_whisper_queue_connection_lost(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_logger_error("yotta_whisper_queue_connection_lost: -> releasing");

    yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);

    yotta_socket_event_release(cmd_queue);
}

void
yotta_whisper_queue_init(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(YOTTA_WHISPER_LABELS_COUNT <= sizeof(yotta_whisper_label_t) * 8);
    yotta_assert(sizeof(yotta_whisper_label_t) == 1);

    yotta_dirty_offset(
        cmd_queue,
        sizeof(cmd_queue->tcp_queue),
        sizeof(*cmd_queue) - sizeof(cmd_queue->tcp_queue)
    );

    yotta_tcp_queue_init((yotta_tcp_queue_t *) cmd_queue);
    yotta_socket_event_set_recv(cmd_queue, yotta_whisper_queue_recv);
    yotta_socket_event_set_except(cmd_queue, yotta_whisper_queue_except);
    yotta_socket_event_set_lost(cmd_queue, yotta_whisper_queue_connection_lost);

    cmd_queue->callback = 0;

    memset(&cmd_queue->recv_buffer, 0, YOTTA_WHISPER_RECV_BUFFER_SIZE);
}

yotta_return_t
yotta_whisper_queue_connect(yotta_whisper_queue_t * cmd_queue, char const * ip, uint16_t port)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(ip != 0);
    yotta_assert(port != 0);

    yotta_dirty_s(cmd_queue);

    if (yotta_tcp_socket_client((yotta_socket_t *) cmd_queue, ip, port) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_whisper_queue_init(cmd_queue);

    return YOTTA_SUCCESS;
}

void
yotta_whisper_queue_destroy(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_tcp_queue_destroy((yotta_tcp_queue_t *) cmd_queue);
}
