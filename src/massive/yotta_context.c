
#include <string.h>

#include "yotta_context.h"
#include "yotta_daemon.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"
#include "../threading/yotta_sync.private.h"

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
