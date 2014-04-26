
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"


/*
 * @infos: defines a push command
 */
typedef struct
yotta_whisper_push_cmd_s
{
    // the TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        yotta_addr_t master_address;
        uint64_t data_size;
    } __attribute__((packed))
    header;

    // the data
    uint64_t data_cursor;
    void const * data;

    // the finish sync object
    yotta_sync_t * sync_finished;
}
yotta_whisper_push_cmd_t;


/*
 * @infos: yotta_whisper_push__cmd_t's release event
 */
static
void
yotta_whisper_push_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    yotta_tcp_cmd_destroy(cmd);
    yotta_free(cmd);
}

/*
 * @infos: yotta_whisper_fetch_request_cmd_t's send event
 */
static
void
yotta_whisper_push_send(yotta_whisper_push_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    // Streams push's header
    yotta_tcp_cmd_stream(
        (yotta_tcp_cmd_t *) cmd,
        sizeof(cmd->header),
        &cmd->header_cursor,
        &cmd->header
    );

    // Streams push's data
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

void
yotta_whisper_push_master_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != 0);

    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_cursor;
        struct
        {
            yotta_addr_t data_address;
            uint64_t data_size;
        }
        header;

        uint64_t data_cursor;
    }
    yotta_whisper_buffer_t;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    if (buffer->header_cursor < sizeof(buffer->header))
    {
        // we are receiving the push's header

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->header),
            &buffer->header_cursor,
            &buffer->header
        );

        if (op != 0)
        {
            return;
        }
    }

    {
        // we are receiving the push's data

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            buffer->header.data_size,
            &buffer->data_cursor,
            (void *) buffer->header.data_address
        );

        if (op != 0)
        {
            return;
        }
    }

    // we clean up the tmp buffer
    buffer->header_cursor = 0;
    buffer->header.data_address = 0;
    buffer->header.data_size = 0;
    buffer->data_cursor = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

static
yotta_tcp_cmd_vtable_t const
yotta_whisper_push_vtable = {
    (yotta_tcp_cmd_entry_t) yotta_whisper_push_release,
    (yotta_tcp_cmd_entry_t) yotta_whisper_push_send
};

void
yotta_whisper_push(
    yotta_whisper_queue_t * cmd_queue,
    yotta_addr_t master_address,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync_finished
)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(master_address != 0);
    yotta_assert(data_size != 0);
    yotta_assert(data != 0);

    /*
     * Inits the finish sync object
     */
    if (sync_finished != 0)
    {
        yotta_sync_init(sync_finished);
    }

    /*
     * Creates the YOTTA_WHISPER_MEM_PUSH command
     */
    yotta_whisper_push_cmd_t * cmd = yotta_alloc_s(yotta_whisper_push_cmd_t);

    yotta_dirty_s(cmd);
    yotta_tcp_cmd_init(cmd, &yotta_whisper_push_vtable);

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_WHISPER_MEM_PUSH;
    cmd->header.master_address = master_address;
    cmd->header.data_size = data_size;
    cmd->data_cursor = 0;
    cmd->data = data;
    cmd->sync_finished = sync_finished;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}
