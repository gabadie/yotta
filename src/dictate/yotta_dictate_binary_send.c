#include <yotta_dictate_binary_send.h>
#include <yotta_return.h>

uint64_t
yotta_dictate_binary_send(yotta_context_t * context, const char * ip, uint16_t port)
{
    yotta_assert(context != NULL);

    //TODO: move to folder Master->Daemon
    char[6] port_str;
    yotta_ui16_to_str(port_str, port);

    uint64_t return_code = yotta_tcp_socket_client(&context->slave, ip, port_str);

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
