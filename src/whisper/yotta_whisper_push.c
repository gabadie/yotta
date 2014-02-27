
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../socket/yotta_tcp.h"


void
yotta_whisper_push_master_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_received;
        uint64_t data_address;
        uint64_t data_size;
        uint64_t data_cursor;
    }
    yotta_whisper_buffer_t;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    if (buffer->header_received < sizeof(uint64_t) * 2)
    {
        // we are receiving the push's header

        uint64_t header_remaining = sizeof(uint64_t) * 2 - buffer->header_received;
        uint64_t header_received = yotta_tcp_recv(
            &cmd_queue->tcp_queue.socket_event.socket,
            ((uint8_t *) &buffer->data_address) + buffer->header_received,
            header_remaining
        );

        if (header_received == -1ull)
        {
            return;
        }

        buffer->header_received += header_received;

        if (buffer->header_received < sizeof(uint64_t) * 2)
        {
            return;
        }

        // we don't need to init the data_cursor as zero because the tmp buffer is null at the begining
        //buffer->data_cursor = 0;
    }

    // we are receiving the push's data

    void * dest = (void *)(buffer->data_address + buffer->data_cursor);
    uint64_t data_remaining = buffer->data_size - buffer->data_cursor;

    uint64_t data_received = yotta_tcp_recv(
        &cmd_queue->tcp_queue.socket_event.socket,
        dest,
        data_remaining
    );

    if (data_received == -1ull)
    {
        return;
    }

    buffer->data_cursor += data_received;

    if (buffer->data_cursor == buffer->data_size)
    {
        // we clean up the tmp buffer
        buffer->header_received = 0;
        buffer->data_address = 0;
        buffer->data_size = 0;
        buffer->data_cursor = 0;

        yotta_whisper_queue_finish(cmd_queue);

        return;
    }
}


typedef struct
yotta_whisper_push_header_s
{
    yotta_whisper_label_t label;
    uint64_t master_address;
    uint64_t data_size;
} __attribute__((packed))
yotta_whisper_push_header_t;

typedef struct
yotta_whisper_push_cmd_s
{
    yotta_tcp_cmd_t abstract_cmd;
    yotta_whisper_push_header_t header;
    uint64_t header_sent;
    void const * data;
    uint64_t data_sent;
}
yotta_whisper_push_cmd_t;

static
void
yotta_whisper_push_send(yotta_whisper_push_cmd_t * cmd, yotta_tcp_queue_t * cmd_queue)
{
    if (cmd->header_sent != sizeof(yotta_whisper_push_header_t))
    {
        ssize_t header_sending = sizeof(yotta_whisper_push_header_t) - cmd->header_sent;

        ssize_t header_sent = yotta_tcp_send(
            (yotta_socket_t *) cmd_queue,
            ((uint8_t *) &cmd->header) + cmd->header_sent,
            header_sending
        );

        if (header_sent == -1)
        {
            return;
        }

        cmd->header_sent += (uint64_t)header_sending;

        if (cmd->header_sent != sizeof(yotta_whisper_push_header_t))
        {
            return;
        }
    }

    ssize_t data_sending = cmd->header.data_size - cmd->data_sent;

    ssize_t data_sent = yotta_tcp_send(
        (yotta_socket_t *) cmd_queue,
        ((uint8_t *) cmd->data) + cmd->data_sent,
        data_sending
    );

    if (data_sent == -1)
    {
        return;
    }

    cmd->data_sent += (uint64_t)data_sent;

    if (cmd->data_sent == cmd->header.data_size)
    {
        yotta_tcp_cmd_dequeue(cmd_queue);
        yotta_tcp_cmd_release(cmd);
    }
}

static
void
yotta_whisper_push_release(yotta_tcp_cmd_t * cmd)
{
    yotta_free(cmd);
}

void
yotta_whisper_push(
    yotta_whisper_queue_t * cmd_queue,
    uint64_t master_address,
    uint64_t data_size,
    void const * data
)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(master_address != 0);
    yotta_assert(data_size != 0);
    yotta_assert(data != 0);

    yotta_whisper_push_cmd_t * cmd = yotta_alloc_s(yotta_whisper_push_cmd_t);

    yotta_tcp_cmd_set_send(cmd, yotta_whisper_push_send);
    yotta_tcp_cmd_set_release(cmd, yotta_whisper_push_release);

    cmd->header.label = YOTTA_WHISPER_MEM_PUSH;
    cmd->header.master_address = master_address;
    cmd->header.data_size = data_size;
    cmd->header_sent = 0;
    cmd->data = data;
    cmd->data_sent = 0;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}
