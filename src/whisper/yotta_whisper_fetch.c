
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"


/*
 * @infos: defines a fetch request command
 */
typedef struct
yotta_whisper_fetch_request_cmd_s
{
    // the TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        uint64_t master_address;
        uint64_t data_size;
        uint64_t data_dest;
    } __attribute__((packed))
    header;
}
yotta_whisper_fetch_request_cmd_t;

/*
 * @infos: defines a fetch answer command
 */
typedef struct
yotta_whisper_fetch_answer_cmd_s
{
    // the tcp command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        uint64_t data_size;
        uint64_t data_dest;
    } __attribute__((packed))
    header;

    // the data
    uint64_t data_cursor;
    void * data;
}
yotta_whisper_fetch_answer_cmd_t;


/*
 * @infos: yotta_whisper_fetch_{answer,request}_cmd_t's release event
 */
static
void
yotta_whisper_fetch_release(yotta_tcp_cmd_t * cmd)
{
    yotta_free(cmd);
}

/*
 * @infos: yotta_whisper_fetch_request_cmd_t's send event
 */
static
void
yotta_whisper_fetch_request_send(yotta_whisper_fetch_request_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        // send fetch request's header

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

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

/*
 * @infos: yotta_whisper_fetch_answer_cmd_t's send event
 */
static
void
yotta_whisper_fetch_answer_send(yotta_whisper_fetch_answer_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        // send fetch answer's header

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
        // send fetch answer's data

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

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

/*
 * @infos: YOTTA_WHISPER_MEM_FETCH_REQUEST's receive event
 */
void
yotta_whisper_fetch_request_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != 0);

    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_received;
        struct
        {
            uint64_t data_address;
            uint64_t data_size;
            uint64_t data_dest;
        }
        header;
    }
    yotta_whisper_buffer_t;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    if (buffer->header_received < sizeof(buffer->header))
    {
        // we are receiving the fetch request's header

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->header),
            &buffer->header_received,
            &buffer->header
        );

        if (op != 0)
        {
            return;
        }
    }

    {
        // generates fetch answer command

        yotta_whisper_fetch_answer_cmd_t * cmd =
            yotta_alloc_s(yotta_whisper_fetch_answer_cmd_t);

        yotta_dirty_s(cmd);

        yotta_tcp_cmd_init(cmd);
        yotta_tcp_cmd_set_send(cmd, yotta_whisper_fetch_answer_send);
        yotta_tcp_cmd_set_release(cmd, yotta_whisper_fetch_release);

        cmd->header_cursor = 0;
        cmd->header.label = YOTTA_WHISPER_MEM_FETCH_ANSWER;
        cmd->header.data_size = buffer->header.data_size;
        cmd->header.data_dest = buffer->header.data_dest;
        cmd->data_cursor = 0;
        cmd->data = (void *) buffer->header.data_address;

        yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
    }

    // we clean up the tmp buffer
    buffer->header_received = 0;
    buffer->header.data_address = 0;
    buffer->header.data_size = 0;
    buffer->header.data_dest = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

/*
 * @infos: YOTTA_WHISPER_MEM_FETCH_ANSWER's receive event
 */
void
yotta_whisper_fetch_answer_recv(
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
            uint64_t data_size;
            uint64_t data_dest;
        }
        header;

        uint64_t data_cursor;
    }
    yotta_whisper_buffer_t;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    if (buffer->header_cursor < sizeof(buffer->header))
    {
        // we are receiving the fetch awnser's header

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
        // we are receiving the fetch awnser's data

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            buffer->header.data_size,
            &buffer->data_cursor,
            (void *) buffer->header.data_dest
        );

        if (op != 0)
        {
            return;
        }
    }

    // we clean up the tmp buffer
    buffer->header_cursor = 0;
    buffer->header.data_size = 0;
    buffer->header.data_dest = 0;
    buffer->data_cursor = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

void
yotta_whisper_fetch(
    yotta_whisper_queue_t * cmd_queue,
    uint64_t master_address,
    uint64_t data_size,
    void * data_dest
)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(master_address != 0);
    yotta_assert(data_size != 0);
    yotta_assert(data_dest != 0);

    yotta_whisper_fetch_request_cmd_t * cmd =
        yotta_alloc_s(yotta_whisper_fetch_request_cmd_t);

    yotta_dirty_s(cmd);

    yotta_tcp_cmd_init(cmd);
    yotta_tcp_cmd_set_send(cmd, yotta_whisper_fetch_request_send);
    yotta_tcp_cmd_set_release(cmd, yotta_whisper_fetch_release);

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_WHISPER_MEM_FETCH_REQUEST;
    cmd->header.master_address = master_address;
    cmd->header.data_size = data_size;
    cmd->header.data_dest = (uint64_t) data_dest;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}