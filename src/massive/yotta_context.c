
#include <string.h>

#include "yotta_context.h"
#include "yotta_daemon.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"

#define YOTTA_CONTEXT_MAX_DEAMONS 32


static
void
yotta_context_release_whisper_queue(yotta_whisper_queue_t * queue)
{
    yotta_assert(queue != NULL);

    yotta_daemon_t * d = yotta_struct_container(yotta_daemon_t, whisper_queue, queue);

    yotta_daemon_destroy(d);
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

    yotta_whisper_queue_t * cmd_queue = NULL;

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
            cmd_queue = &d->whisper_queue;
            break;
        }
    }

    if (cmd_queue == NULL)
    {
        yotta_logger_warning("yotta_context_create_whisper_queue: unknown deamon");
        yotta_socket_close(&socket);
        return;
    }

    // TODO: daemon already connected?

    yotta_dirty_s(cmd_queue);

    cmd_queue->tcp_queue.socket_event.socket.fd = socket.fd;

    yotta_whisper_queue_init(cmd_queue);
    yotta_socket_event_set_release((yotta_socket_event_t *) cmd_queue, yotta_context_release_whisper_queue);

    if (yotta_socket_thread_listen(master->socket_event.socket_thread, (yotta_socket_event_t *) cmd_queue) != 0)
    {
        yotta_socket_event_release(cmd_queue);
    }

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

    yotta_socket_thread_init(&context->worker_thread);

    context->daemons = yotta_alloc_sa(yotta_daemon_t, YOTTA_CONTEXT_MAX_DEAMONS);

    for (size_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        context->daemons[i].context = NULL;
    }

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

    yotta_return_t r = yotta_daemon_init(uninit_deamon, context, ip, port);

    if (r == YOTTA_UNEXPECTED_FAIL)
    {
        yotta_return_unexpect_fail(yotta_context_connect);
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
