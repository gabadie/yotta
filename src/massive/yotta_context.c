
#include "yotta_context.h"
#include "yotta_daemon.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"

#define YOTTA_CONTEXT_MAX_DEAMONS 32


yotta_return_t
yotta_context_init(yotta_context_t * context, uint16_t incoming_port)
{
    if (context == 0)
    {
        yotta_return_inv_value(yotta_context_init, context);
    }

    if (yotta_whisper_master_init(&context->whisper_master, incoming_port) != 0)
    {
        context->daemons = NULL;

        yotta_return_unexpect_fail(yotta_context_destroy)
    }

    yotta_socket_thread_init(&context->worker_thread);

    context->daemons = yotta_alloc_sa(yotta_daemon_t, YOTTA_CONTEXT_MAX_DEAMONS);

    for (uint64_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        context->daemons[i].context = NULL;
    }

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_context_connect(yotta_context_t * context, char const * ip, uint16_t port)
{
    if (context == 0)
    {
        yotta_return_inv_value(yotta_context_connect, context);
    }
    else if (ip == 0)
    {
        yotta_return_inv_value(yotta_context_connect, ip);
    }
    else if (port == 0)
    {
        yotta_return_inv_value(yotta_context_connect, port);
    }

    yotta_daemon_t * uninit_deamon = NULL;

    for (uint64_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
    {
        if (context->daemons[i].context == NULL)
        {
            uninit_deamon = context->daemons + i;
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
    if (context == 0)
    {
        yotta_return_inv_value(yotta_context_destroy, context);
    }

    yotta_return_t r = YOTTA_SUCCESS;

    if (context->daemons != NULL)
    {
        for (uint64_t i = 0; i < YOTTA_CONTEXT_MAX_DEAMONS; i++)
        {
            if (context->daemons[i].context == NULL)
            {
                continue;
            }

            r |= yotta_daemon_destroy(context->daemons + i);
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

    if (r != YOTTA_SUCCESS)
    {
        yotta_return_unexpect_fail(yotta_context_destroy);
    }

    return YOTTA_SUCCESS;
}
