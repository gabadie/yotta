#include <stdio.h>

#include "yotta_dictate_binary_send.h"
#include "../yotta_debug.h"
#include "../yotta_init.h"
#include "../core/yotta_return.h"
#include "../core/yotta_logger.private.h"
#include "../utils/yotta_str_utils.h"
#include "../socket/yotta_tcp.h"

#define BINARY_BUFFER_SIZE 1024

uint64_t
yotta_dictate_binary_send(yotta_context_t * context, char const * ip, uint16_t port)
{
    yotta_assert(context != NULL);

    uint64_t return_code = yotta_tcp_socket_client(&context->slave, ip, port);

    if(return_code != YOTTA_SUCCESS)
    {
        yotta_logger_error("Failed to create tcp socket");
        return return_code;
    }

    FILE * binary;
    uint8_t buffer[BINARY_BUFFER_SIZE];

    binary = fopen(yotta_executable_path, "rb");
    if(binary == NULL)
    {
        yotta_logger_error("Failed to open binary");
        return YOTTA_INVALID_VALUE;
    }

    //TODO
    /*yotta_tcp_send(&context->slave,*/

    size_t read_size = 0;
    while(!feof(binary))
    {
        read_size = fread(buffer, BINARY_BUFFER_SIZE, 1, binary);
        yotta_tcp_send(&context->slave, buffer, read_size);
    }

    fclose(binary);
    return YOTTA_SUCCESS;
}
