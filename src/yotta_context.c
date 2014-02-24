#include <stdio.h>

#include "yotta_context.h"
#include "yotta_init.h"
#include "yotta_logger.private.h"
#include "yotta_return.h"
#include "utils/yotta_str_utils.h"
#include "dictate/yotta_dictate_binary_send.h"

#define BINARY_BUFFER_SIZE = 1024

uint64_t
yotta_context_init(yotta_context_t * context, uint16_t incoming_port)
{
    yotta_assert(context != NULL);

    memset(context, 0, sizeof(yotta_context_t));

    context->incoming_port = incoming_port;

    return YOTTA_SUCCESS;
}

uint64_t
yotta_context_connect(yotta_context_t * context, const char * ip, uint16_t port)
{
    return yotta_dictate_binary_send(context, ip, port);
}

uint64_t
yotta_context_destroy(yotta_context_t * context)
{
    yotta_assert(context != NULL);

    return yotta_socket_close(&context->client);
}
