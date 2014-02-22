
#include "yotta_whisper_labels.private.h"
#include "../yotta_debug.h"


void
yotta_whisper_entry_push(
    yotta_context_t * context,
    yotta_socket_t * socket,
    yotta_whisper_entry_feedback_t * feedback
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

    (void) context;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_tmp_buffer(feedback, yotta_whisper_buffer_t);

    if (buffer->header_received < sizeof(uint64_t) * 2)
    {
        // we are receiving the push's header

        uint64_t header_remaining = sizeof(uint64_t) * 2 - buffer->header_received;

        buffer->header_received += yotta_tcp_recv(
            socket,
            &buffer->data_address,
            header_remaining
        );

        if (buffer->header_received < sizeof(uint64_t) * 2)
        {
            // we din't fully received the push's header
            yotta_whisper_feedback_continue(feedback, yotta_whisper_entry_push);
            return;
        }

        // we don't need to init the data_cursor as zero because the tmp buffer is null at the begining
        //buffer->data_cursor = 0;
    }

    // we are receiving the push's data

    void * dest = (void *)(buffer->data_address + buffer->data_cursor);
    uint64_t data_remaining = buffer->data_size - buffer->data_cursor;

    buffer->data_cursor += yotta_tcp_recv(socket, dest, data_remaining);

    if (buffer->data_cursor == buffer->data_cursor)
    {
        // we clean up the tmp buffer
        buffer->header_received = 0;
        buffer->data_address = 0;
        buffer->data_size = 0;
        buffer->data_cursor = 0;


        yotta_whisper_feedback_finish(feedback);

        return;
    }

    yotta_whisper_feedback_continue(feedback, yotta_whisper_entry_push);
}

void
yotta_whisper_push(
    yotta_socket_t * socket,
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

    yotta_tcp_send(socket, &header, sizeof(header));
    yotta_tcp_send(socket, data, data_size);
}
