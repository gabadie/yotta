
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_labels.private.h"
#include "../socket/yotta_tcp.h"
#include "../yotta_debug.h"


void
yotta_whisper_entry_push(
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

        yotta_whisper_queue_continue(cmd_queue, yotta_whisper_entry_push);

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

void
yotta_whisper_push(
    yotta_whisper_queue_t * cmd_queue,
    uint64_t master_address,
    uint64_t data_size,
    void const * data
)
{
    struct
    {
        uint16_t label;
        uint64_t master_address;
        uint64_t data_size;
    } __attribute__((packed)) header;

    header.label = YOTTA_WHISPER_MEM_PUSH;
    header.master_address = master_address;
    header.data_size = data_size;

    yotta_assert(socket != 0);
    yotta_assert(data_size != 0);
    yotta_assert(data != 0);

    yotta_tcp_send(&cmd_queue->tcp_queue.socket_event.socket, &header, sizeof(header));
    yotta_tcp_send(&cmd_queue->tcp_queue.socket_event.socket, data, data_size);
}
