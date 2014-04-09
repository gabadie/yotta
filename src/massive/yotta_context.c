
#include <string.h>

#include "yotta_context.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"


yotta_return_t
yotta_context_init(yotta_context_t * context, uint16_t incoming_port)
{
    if (context == 0)
    {
        yotta_return_inv_value(yotta_context_init, context);
    }

    if (yotta_whisper_master_init(&context->whisper_master, incoming_port) != 0)
    {
        yotta_return_unexpect_fail(yotta_context_destroy)
    }

    yotta_socket_thread_init(&context->worker_thread);

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

    yotta_todo("Implements dicate protocol");

    yotta_return_unexpect_fail(yotta_context_connect)
}

yotta_return_t
yotta_context_destroy(yotta_context_t * context)
{
    if (context == 0)
    {
        yotta_return_inv_value(yotta_context_destroy, context);
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
