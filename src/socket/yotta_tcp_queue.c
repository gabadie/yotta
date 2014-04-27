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

#include "yotta_tcp.h"
#include "yotta_tcp_queue.private.h"
#include "yotta_socket_thread.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"


/*
 * Moves the entire commands' stack content at the tail of the command list
 */
static
void
yotta_tcp_queue_pop_stack(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(cmd_queue->socket_event.socket_thread != 0);

    yotta_tcp_cmd_t * queue_stack;

    do // atomic commands stack fetching
    {
        queue_stack = cmd_queue->queue_stack;


        if (queue_stack == 0)
        {
            return;
        }
    }
    while (!__sync_bool_compare_and_swap(&cmd_queue->queue_stack, queue_stack, 0));

    yotta_assert(queue_stack != 0);


    // sets the new commands queue's end as the first of the commands stack
    cmd_queue->queue_last = queue_stack;

    // reverses queue_stack into queue_append
    yotta_tcp_cmd_t * queue_append = 0;

    while (queue_stack)
    {
        yotta_tcp_cmd_t * cmd = queue_stack;

        queue_stack = queue_stack->queue_next;
        cmd->queue_next = queue_append;
        queue_append = cmd;
    }

    // appends queue_append to the back of comamnds queue
    if (cmd_queue->queue_first == 0)
    {
        cmd_queue->queue_first = queue_append;
    }
    else
    {
        cmd_queue->queue_last->queue_next = queue_append;
    }
}

static
void
yotta_tcp_queue_send(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_tcp_queue_pop_stack(cmd_queue);

    yotta_tcp_cmd_t * cmd = cmd_queue->queue_first;

    if (cmd == 0)
    {
        yotta_socket_event_set_send(cmd_queue, 0);
        return;
    }

    yotta_vcall(cmd, yotta_tcp_cmd_vtable_t, stream);

    /*
     * We should not be doing something else on cmd because it might have killed
     * itself that could probably append if it has just finished.
     *
     * We do not do something else on cmd_queue either because it might have
     * been killed too.
     */
}

void
yotta_tcp_queue_init(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_dirty_offset(
        cmd_queue,
        sizeof(cmd_queue->socket_event),
        sizeof(*cmd_queue) - sizeof(cmd_queue->socket_event)
    );

    yotta_socket_nonblock((yotta_socket_t *) cmd_queue);

    yotta_socket_event_init(cmd_queue);
    yotta_socket_event_set_send(cmd_queue, 0);

#ifdef YOTTA_DEBUG
    yotta_socket_event_set_recv(cmd_queue, 0);
    yotta_socket_event_set_except(cmd_queue, 0);
    yotta_socket_event_set_release(cmd_queue, 0);
#endif // YOTTA_DEBUG

    cmd_queue->queue_first = 0;
    cmd_queue->queue_stack = 0;
}

void
yotta_tcp_queue_append(yotta_tcp_queue_t * cmd_queue, yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(cmd != 0);
    yotta_assert(cmd->queue == 0);

    cmd->queue = cmd_queue;

    do // atomic commands stack pushing
    {
        cmd->queue_next = cmd_queue->queue_stack;
    }
    while (!__sync_bool_compare_and_swap(&cmd_queue->queue_stack, cmd->queue_next, cmd));

    yotta_socket_event_set_send(cmd_queue, yotta_tcp_queue_send);
}

uint64_t
yotta_tcp_queue_recv(yotta_tcp_queue_t * cmd_queue, uint64_t buffer_size, uint64_t * buffer_cursor, void * buffer)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(buffer_size != 0);
    yotta_assert(buffer_cursor != 0);
    yotta_assert(buffer != 0);
    yotta_assert(*buffer_cursor < buffer_size);

    ssize_t buffer_receiving = buffer_size - *buffer_cursor;

    ssize_t buffer_received = yotta_tcp_recv(
        (yotta_socket_t *) cmd_queue,
        ((uint8_t *) buffer) + *buffer_cursor,
        buffer_receiving
    );

    if (buffer_received == -1)
    {
        if (errno == EAGAIN)
        {
            // no incomming information available
            return 1;
        }
        else if (errno == ECONNRESET)
        {
            // The connection is closed by the peer during a receive attempt on a socket.
            yotta_socket_event_lost(cmd_queue);

            return 1;
        }

        // unhandled error
        yotta_crash_msg("yotta_tcp_queue_recv unhandled error");

        return 1;
    }
    else if (buffer_received == 0)
    {
        // The connection is closed by the peer during a receive attempt on a socket.

        yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);
        yotta_socket_event_release(cmd_queue);

        return 1;
    }

    *buffer_cursor += (uint64_t)buffer_received;

    yotta_assert(*buffer_cursor <= buffer_size);

    return *buffer_cursor != buffer_size;
}

static
void
yotta_tcp_queue_finish_send(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(yotta_tcp_cmd_queue(cmd));

    yotta_socket_event_t * socket_event = (yotta_socket_event_t * )yotta_tcp_cmd_queue(cmd);

    yotta_socket_event_unlisten(socket_event);

    yotta_socket_event_release(socket_event);
}

static
void
yotta_tcp_queue_finish_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    yotta_tcp_cmd_destroy(cmd);
    yotta_free(cmd);
}

static
yotta_tcp_cmd_vtable_t const
tcp_queue_finish_vtable = {
    yotta_tcp_queue_finish_release,
    yotta_tcp_queue_finish_send
};

void
yotta_tcp_queue_finish(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(cmd_queue->socket_event.socket_thread != 0);

    yotta_tcp_cmd_t * cmd = yotta_alloc_s(yotta_tcp_cmd_t);

    yotta_tcp_cmd_init(cmd, &tcp_queue_finish_vtable);

    yotta_tcp_queue_append(cmd_queue, cmd);
}

void
yotta_tcp_queue_destroy(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(cmd_queue->socket_event.socket_thread == 0);

    while (cmd_queue->queue_first)
    {
        yotta_tcp_cmd_t * cmd = cmd_queue->queue_first;

        cmd_queue->queue_first = cmd->queue_next;

#ifdef YOTTA_ASSERT
        cmd->queue = NULL;
#endif

        yotta_vcall_release(cmd);
    }

    yotta_socket_event_destroy(cmd_queue);
}
