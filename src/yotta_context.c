#include <stdio.h>
#include <string.h>

#include "yotta_context.h"
#include "yotta_debug.h"
#include "yotta_return.h"
#include "dictate/yotta_dictate_binary_send.h"
#include "socket/yotta_socket.h"

uint64_t
yotta_context_init(yotta_context_t * context, yotta_thread_func_t func,
    yotta_thread_args_t args, uint16_t incoming_port)
{
    yotta_assert(context != NULL);

    memset(context, 0, sizeof(yotta_context_t));

    context->incoming_port = incoming_port;

    yotta_socket_thread_init(&context->worker_thread, func, args);

    return YOTTA_SUCCESS;
}

uint64_t
yotta_context_connect(yotta_context_t * context, char const * ip, uint16_t port)
{
    return yotta_dictate_binary_send(context, ip, port);

}

uint64_t
yotta_context_destroy(yotta_context_t * context)
{
    yotta_assert(context != NULL);

    return yotta_close_socket(&context->slave);
}
