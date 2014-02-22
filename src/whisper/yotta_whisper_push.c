
#include "yotta_whisper_labels.private.h"
#include "../yotta_debug.h"


void
yotta_whisper_entry_push(
    yotta_context_t * context,
    yotta_socket_t * socket,
    yotta_whisper_entry_feedback_t * feedback,
    void * tmp_buffer_ptr
)
{
    (void) context;
    (void) socket;
    (void) feedback;
    (void) tmp_buffer_ptr;
}

void
yotta_whisper_push(
    yotta_socket_t * socket,
    uint64_t master_address,
    uint64_t data_size,
    void * const data
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
