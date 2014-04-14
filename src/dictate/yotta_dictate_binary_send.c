#include <stdio.h>
#include <string.h>

#include "yotta_dictate_binary_send.private.h"
#include "yotta_dictate_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.h"
#include "../core/yotta_logger.private.h"
#include "../socket/yotta_tcp.h"
#include "../threading/yotta_sync.private.h"

#define BINARY_BUFFER_SIZE 4096


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

    // File handler of the binary
    FILE * binary_file;

    // Cursor on the binary's data
    uint64_t data_cursor;

    // Last size of data read
    size_t data_read;

    // Temporary buffer for binary send
    uint8_t data[BINARY_BUFFER_SIZE];

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
    yotta_assert(cmd != NULL);

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
    yotta_assert(cmd->binary_file != NULL);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        // Send frame header

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
        // Send data

        while (!feof(cmd->binary_file))
        {
            cmd->data_read = fread(cmd->data, BINARY_BUFFER_SIZE, 1, cmd->binary_file);

            uint64_t op = yotta_tcp_cmd_send(
                (yotta_tcp_cmd_t *) cmd,
                cmd->data_read,
                &cmd->data_cursor,
                cmd->data
            );

            if (op != 0)
            {
                return;
            }

            cmd->data_cursor = 0;
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

    fclose(cmd->binary_file);
    yotta_tcp_cmd_release(cmd);
}


uint64_t
yotta_dictate_binary(
    yotta_dictate_queue_t * cmd_queue,
    char const * executable_path,
    yotta_sync_t * sync_finished
)
{
    yotta_assert(cmd_queue != NULL);

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

    // Initialize the command
    yotta_tcp_cmd_init(cmd);
    yotta_tcp_cmd_set_send(cmd, yotta_dictate_binary_send);
    yotta_tcp_cmd_set_release(cmd, yotta_dictate_binary_release);

    cmd->binary_file = fopen(executable_path, "rb");

    if(cmd->binary_file == NULL)
    {
        yotta_logger_error("Failed to open binary");
        return YOTTA_INVALID_VALUE;
    }

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_DICTATE_LABEL_SLAVE_BINARY;

    fseek(cmd->binary_file, 0, SEEK_END);
    cmd->header.data_size = ftell(cmd->binary_file);
    rewind(cmd->binary_file);

    cmd->data_cursor = 0;
    cmd->data_read = 0;
    memset(cmd->data, 0, sizeof(cmd->data));
    cmd->sync_finished = sync_finished;



    // Queue the command
    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);

    return YOTTA_SUCCESS;
}
