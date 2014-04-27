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

#include "yotta_dictate_queue.private.h"
#include "yotta_dictate_daemon_info.private.h"
#include "yotta_dictate_daemon_error.private.h"
#include "yotta_dictate_unknown.private.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_debug.h"
#include "../socket/yotta_tcp.h"
#include "../socket/yotta_socket_thread.h"


#ifdef YOTTA_DEBUG

/*
 * @infos: verify that the receive buffer has been cleaned up
 *
 * @param <cmd_queue>: the command queue to verify
 */
static
void
yotta_dictate_queue_assert_buffer(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    for (size_t i = 0; i < sizeof(cmd_queue->recv_buffer); i++)
    {
        yotta_assert(cmd_queue->recv_buffer[i] == 0);
    }
}

#endif //YOTTA_DEBUG


/*
 * Default vtable for yotta_dictate_queue_s
 */
static
yotta_dictate_vtable_t const
yotta_dictate_queue_default_vtable =
{
    yotta_dictate_vtable_daemon_info_recv,
    yotta_dictate_vtable_daemon_error_recv,
    yotta_dictate_vtable_unknown_recv
};

static
void
yotta_dictate_queue_recv(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != NULL);

    while (1)
    {
        if (cmd_queue->callback != NULL)
        {
            cmd_queue->callback(cmd_queue);

            if (cmd_queue->callback != NULL)
            {
                // the callback has not finished to receive all data
                break;
            }

#ifdef YOTTA_DEBUG
            yotta_dictate_queue_assert_buffer(cmd_queue);
#endif //YOTTA_DEBUG
        }

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(cmd_queue->header),
            &cmd_queue->header_cursor,
            &cmd_queue->header
        );

        if (op != 0)
        {
            return;
        }

        cmd_queue->header_cursor = 0;

        switch (cmd_queue->header.label)
        {
            case YOTTA_DICTATE_LABEL_DAEMON_INFO:
            {
                cmd_queue->callback = yotta_dictate_daemon_info_recv;
                break;
            }

            case YOTTA_DICTATE_LABEL_DAEMON_ERROR:
            {
                cmd_queue->callback = yotta_dictate_daemon_error_recv;
                break;
            }

            default:
            {
                /*cmd_queue->callback = yotta_dictate_unknown_label;*/
            }
        }
    }
}

static
void
yotta_dictate_queue_except(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_logger_error("yotta_dictate_queue_except: received a TCP socket exception -> releasing");

    yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);

    yotta_socket_event_release(cmd_queue);
}

static
void
yotta_dictate_queue_connection_lost(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_logger_error("yotta_dictate_queue_connection_lost: -> releasing");

    yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);

    yotta_socket_event_release(cmd_queue);
}

void
yotta_dictate_queue_init(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(sizeof(yotta_dictate_label_t) == 2);

    yotta_dirty_offset(
        cmd_queue,
        sizeof(cmd_queue->tcp_queue),
        sizeof(*cmd_queue) - sizeof(cmd_queue->tcp_queue)
    );

    yotta_tcp_queue_init((yotta_tcp_queue_t *) cmd_queue);
    yotta_socket_event_set_recv(cmd_queue, yotta_dictate_queue_recv);
    yotta_socket_event_set_except(cmd_queue, yotta_dictate_queue_except);
    yotta_socket_event_set_lost(cmd_queue, yotta_dictate_queue_connection_lost);

    cmd_queue->callback = NULL;

    memset(&cmd_queue->header, 0, sizeof(cmd_queue->header));

    cmd_queue->header_cursor = 0;

    memset(&cmd_queue->recv_buffer, 0, YOTTA_DICTATE_RECV_BUFFER_SIZE);

    cmd_queue->vtable = &yotta_dictate_queue_default_vtable;
}

yotta_return_t
yotta_dictate_queue_connect(yotta_dictate_queue_t * cmd_queue, char const * ip, uint16_t port)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(ip != NULL);
    yotta_assert(port != 0);

    yotta_dirty_s(cmd_queue);

    if (yotta_tcp_socket_client((yotta_socket_t *) cmd_queue, ip, port) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_dictate_queue_init(cmd_queue);

    return YOTTA_SUCCESS;
}

void
yotta_dictate_queue_destroy(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != NULL);

    yotta_tcp_queue_destroy((yotta_tcp_queue_t *) cmd_queue);
}
