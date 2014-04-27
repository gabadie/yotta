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

#include "yotta_context.h"
#include "yotta_daemon.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"
#include "../threading/yotta_sync.private.h"
#include "../whisper/yotta_whisper_command.private.h"

#define YOTTA_CONTEXT_MAX_DEAMONS 32


static
void
yotta_context_release_whisper_master(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);

    yotta_logger_warning("yotta_context_release_whisper_master");

    yotta_not_implemented_yet();

    yotta_whisper_master_destroy(master);
}

static
void
yotta_context_release_whisper_queue(yotta_whisper_queue_t * queue)
{
    yotta_assert(queue != NULL);

    yotta_daemon_t * d = yotta_struct_container(yotta_daemon_t, whisper_queue, queue);

    d->status &= ~YOTTA_DAEMON_WHISPER_READY;

    yotta_whisper_queue_destroy(queue);
}

static
void
yotta_context_create_whisper_queue(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);

    yotta_context_t * context = yotta_struct_container(yotta_context_t, whisper_master, master);

    yotta_assert(&context->whisper_master == master);

    yotta_socket_t socket;

    if (yotta_socket_accept(&master->socket_event.socket, &socket) != 0)
    {
        yotta_logger_warning("yotta_context_create_whisper_queue: connection has failed");
        return;
    }

    yotta_ipaddr_t incomming_ip_address;

    if (yotta_socket_peer(&socket, incomming_ip_address, NULL))
    {
        yotta_logger_warning("yotta_context_create_whisper_queue: unknown IP address");
        yotta_socket_close(&socket);
        return;
    }

    yotta_daemon_t * current_daemon = NULL;

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        yotta_daemon_t * d = context->daemons + i;

        if (d->context == NULL)
        {
            continue;
        }

        yotta_ipaddr_t ip_address;

        if (yotta_socket_peer(&d->dictate_queue.tcp_queue.socket_event.socket, ip_address, NULL))
        {
            yotta_logger_warning("yotta_context_create_whisper_queue: unknown dictate IP address");
            continue;
        }

        if (strcmp(ip_address, incomming_ip_address) == 0)
        {
            current_daemon = d;
            break;
        }
    }

    if (current_daemon == NULL)
    {
        yotta_logger_warning("yotta_context_create_whisper_queue: unknown deamon");
        yotta_socket_close(&socket);
        return;
    }

    yotta_assert((current_daemon->status & YOTTA_DAEMON_WHISPER_READY) == 0x0);

    yotta_whisper_queue_t * whisper_queue = &current_daemon->whisper_queue;

    // TODO: daemon already connected?

    yotta_dirty_s(whisper_queue);

    whisper_queue->tcp_queue.socket_event.socket.fd = socket.fd;

    yotta_whisper_queue_init(whisper_queue);
    yotta_socket_event_set_release((yotta_socket_event_t *) whisper_queue, yotta_context_release_whisper_queue);

    if (yotta_socket_thread_listen(master->socket_event.socket_thread, (yotta_socket_event_t *) whisper_queue) != 0)
    {
        yotta_logger_warning("yotta_context_create_whisper_queue: failed to listen daemon");
        yotta_socket_event_release(whisper_queue);
        return;
    }

    current_daemon->status |= YOTTA_DAEMON_WHISPER_READY;

    yotta_logger_debug("yotta_context_create_whisper_queue: daemon's whisper ready");
    yotta_sync_post(&current_daemon->sync_whisper_ready);

    return;
}

static
yotta_whisper_master_vtable_t const
yotta_context_whisper_vtable = {
    yotta_context_create_whisper_queue
};


yotta_return_t
yotta_context_init(yotta_context_t * context, uint16_t incoming_port)
{
    if (context == NULL)
    {
        yotta_return_inv_value(yotta_context_init, context);
    }

    if (yotta_whisper_master_init(&context->whisper_master, &yotta_context_whisper_vtable, incoming_port) != 0)
    {
        context->daemons = NULL;

        yotta_return_unexpect_fail(yotta_context_destroy)
    }

    yotta_socket_listen(&context->whisper_master.socket_event.socket, 16);

    yotta_socket_event_set_release(&context->whisper_master, yotta_context_release_whisper_master);

    yotta_socket_thread_init(&context->worker_thread);
    yotta_socket_thread_listen(&context->worker_thread, (yotta_socket_event_t *) &context->whisper_master);

    context->daemons = yotta_alloc_sa(yotta_daemon_t, YOTTA_CONTEXT_MAX_DEAMONS);

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        context->daemons[i].context = NULL;
    }

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_context_deamons_count(yotta_context_t * context, uint64_t * out_count)
{
    if (context == NULL)
    {
        yotta_return_inv_value(yotta_context_init, context);
    }
    else if (out_count == NULL)
    {
        yotta_return_inv_value(yotta_context_init, out_count);
    }

    uint64_t deamons_count = 0;

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        if (context->daemons[i].context)
        {
            deamons_count++;
        }
    }

    *out_count = deamons_count;

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_context_connect(yotta_context_t * context, char const * ip, uint16_t port)
{
    if (context == NULL)
    {
        yotta_return_inv_value(yotta_context_connect, context);
    }
    else if (ip == NULL)
    {
        yotta_return_inv_value(yotta_context_connect, ip);
    }
    else if (port == 0)
    {
        yotta_return_inv_value(yotta_context_connect, port);
    }

    yotta_daemon_t * uninit_deamon = NULL;

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        if (context->daemons[i].context == NULL)
        {
            uninit_deamon = context->daemons + i;
            uninit_deamon->context = context;
            break;
        }
    }

    if (uninit_deamon == NULL)
    {
        yotta_return_inv_op(yotta_context_connect);
    }

    yotta_logger_debug("yotta_context_connect: connecting ...");

    yotta_return_t r = yotta_daemon_init(uninit_deamon, context, ip, port);

    if (r == YOTTA_UNEXPECTED_FAIL)
    {
        uninit_deamon->context = NULL;
        yotta_return_unexpect_fail(yotta_context_connect);
    }

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_context_massive(
    yotta_context_t * context,
    yotta_massive_command_entry_t function_addr,
    uint64_t param_size,
    void const * param,
    uint64_t stride
)
{
    if (context == NULL)
    {
        yotta_return_inv_value(yotta_context_massive, context);
    }

    uint64_t group_count = 0;
    uint64_t global_count = 0;

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        if (context->daemons[i].context != NULL)
        {
            yotta_assert(
                context->daemons[i].available_computers == 1
            );

            group_count++;
            global_count += context->daemons[i].available_threads;
        }
    }

    uint64_t group_id = 0;
    uint64_t global_offset = 0;

    yotta_sync_t * syncs = yotta_alloc_sa(yotta_sync_t, 2 * group_count);

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        if (context->daemons[i].context == NULL)
        {
            continue;
        }

        yotta_whisper_command(
            &context->daemons[i].whisper_queue,
            function_addr,
            param_size,
            ((uint8_t *) param) + i * stride,
            group_id,
            group_count,
            global_offset,
            global_count,
            &syncs[2*group_id],
            &syncs[2*group_id+1]
        );

        group_id++;
        global_offset += context->daemons[i].available_threads;
    }

    // Synchronizations
    group_id = 0;
    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        if (context->daemons[i].context == NULL)
        {
            continue;
        }

        yotta_sync_wait(&syncs[2*group_id]);
        yotta_sync_wait(&syncs[2*group_id+1]);
        group_id++;
    }

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_context_destroy(yotta_context_t * context)
{
    if (context == NULL)
    {
        yotta_return_inv_value(yotta_context_destroy, context);
    }

    if (context->daemons != NULL)
    {
        for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
        {
            if (context->daemons[i].context == NULL)
            {
                continue;
            }

            yotta_daemon_destroy(context->daemons + i);
        }

        yotta_free(context->daemons);
    }

    yotta_socket_event_unlisten(&context->whisper_master.socket_event);

    if (yotta_whisper_master_destroy(&context->whisper_master) != 0)
    {
        yotta_return_unexpect_fail(yotta_context_destroy)
    }

    if (yotta_socket_thread_destroy(&context->worker_thread) != 0)
    {
        yotta_return_unexpect_fail(yotta_context_destroy);
    }

    return YOTTA_SUCCESS;
}
