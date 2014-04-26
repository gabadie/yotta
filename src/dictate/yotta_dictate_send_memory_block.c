#include <string.h>

#include "yotta_dictate_send_memory_block.private.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.h"
#include "../socket/yotta_tcp_cmd.private.h"
#include "../threading/yotta_sync.private.h"

typedef struct
yotta_dictate_send_memory_block_cmd_s
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

    // The data
    uint64_t data_cursor;
    void const * data;

    // The finish sync object
    yotta_sync_t * sync_finished;
}
yotta_dictate_send_memory_block_cmd_t;

/*
 * @infos: yotta_dictate daemon start release event
 */
static
void
yotta_dictate_send_memory_block_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    yotta_tcp_cmd_destroy(cmd);
    yotta_free(cmd);
}

/*
 * @infos: yotta_dictate daemon start send event
 */
static
void
yotta_dictate_send_memory_block_send(yotta_dictate_send_memory_block_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);
    yotta_assert(cmd->abstract_cmd.queue != NULL);

    // Streams frame header
    yotta_tcp_cmd_stream(
        (yotta_tcp_cmd_t *) cmd,
        sizeof(cmd->header),
        &cmd->header_cursor,
        &cmd->header
    );

    // Streams data
    yotta_tcp_cmd_stream_last(
        (yotta_tcp_cmd_t *) cmd,
        cmd->header.data_size,
        &cmd->data_cursor,
        cmd->data
    );

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

static
yotta_tcp_cmd_vtable_t const
yotta_dictate_send_memory_block_vtable = {
    (yotta_tcp_cmd_entry_t) yotta_dictate_send_memory_block_release,
    (yotta_tcp_cmd_entry_t) yotta_dictate_send_memory_block_send
};

void
yotta_dictate_send_memory_block(
    yotta_dictate_queue_t * cmd_queue,
    yotta_dictate_label_t label,
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
     * Create the yotta binary send command +
     * allocate a buffer for the data
     */
    yotta_dictate_send_memory_block_cmd_t * cmd =
        yotta_alloc_d(
            sizeof(yotta_dictate_send_memory_block_cmd_t) + data_size
        );

    yotta_dirty_s(cmd);

    // Initialize the command
    yotta_tcp_cmd_init(cmd, &yotta_dictate_send_memory_block_vtable);

    cmd->header_cursor = 0;
    cmd->header.label = label;
    cmd->header.data_size = data_size;
    cmd->data_cursor = 0;
    cmd->data = (cmd + 1);
    cmd->sync_finished = sync_finished;

    // Copy the data to the allocated buffer
    memcpy((cmd + 1), data, data_size);

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}
