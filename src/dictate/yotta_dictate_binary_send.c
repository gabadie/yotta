#include <stdio.h>

#include "yotta_dictate_binary_send.private.h"
#include "yotta_dictate_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.h"
#include "../core/yotta_logger.private.h"
#include "../socket/yotta_tcp.h"
#include "../threading/yotta_sync.private.h"

#define BINARY_BUFFER_SIZE 1024


typedef struct
yotta_dictate_binary_send_cmd_s
{
    // TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // The header
    uint64_t header_cursor;
    struct
    {
        yotta_dictate_label_t label;
        uint64_t data_size;
    } __attribute__((packed))
    header;

    // The data (binary)
    uint64_t data_cursor;
    void const * data;

    // The finish sync object
    yotta_sync_t * sync_finished;
}
yotta_dictate_binary_cmd_t;

/*
 * @infos: yotta_dictate binary release event
 */
static
void
yotta_dictate_binary_release(yotta_tcp_cmd_t * cmd)
{
    yotta_free(cmd);
}

/*
 * @infos: yotta_dictate binary send event
 */
static
void
yotta_dictate_binary_send(yotta_dictate_binary_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);
    yotta_assert(cmd->abstract_cmd.queue != NULL);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        // send push's header

        uint64_t op = yotta_tcp_cmd_send(
            (yotta_tcp_cmd_t *) cmd,
            sizeof(cmd->header),
            &cmd->header_cursor,
            &cmd->header
        );

        if (op != 0)
        {
            return;
        }
    }

    {
        // send push's data

        uint64_t op = yotta_tcp_cmd_send(
            (yotta_tcp_cmd_t *) cmd,
            cmd->header.data_size,
            &cmd->data_cursor,
            cmd->data
        );

        if (op != 0)
        {
            return;
        }
    }

    /*
     * We push the sync event and destroy this command
     */
    if (cmd->sync_finished)
    {
        yotta_sync_post(cmd->sync_finished);
    }

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}


void yotta_dictate_binary(
    yotta_dictate_queue_t * cmd_queue,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync_finished
)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(data_size != 0);
    yotta_assert(data != NULL);

    // Initialize the finish sync object
    if (sync_finished != NULL)
    {
        yotta_sync_init(sync_finished);
    }

    /*
     * Create the yotta binary send command
     */
    yotta_dictate_binary_cmd_t * cmd = yotta_alloc_s(yotta_dictate_binary_cmd_t);

    yotta_dirty_s(cmd);

    yotta_tcp_cmd_init(cmd);
    yotta_tcp_cmd_set_send(cmd, yotta_dictate_binary_send);
    yotta_tcp_cmd_set_release(cmd, yotta_dictate_binary_release);

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_DICTATE_LABEL_SLAVE_BINARY;
    cmd->header.data_size = data_size;
    cmd->data = data;
    cmd->sync_finished = sync_finished;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}


#if 0
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
#endif
