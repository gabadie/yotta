#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "yotta_dictate_unknown.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"

#define YOTTA_DICTATE_UNKNOWN_SIZE 256

void
yotta_dictate_unknown_recv(
    yotta_dictate_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(cmd_queue->vtable != NULL);

    // Daemon info struct
    typedef struct
    yotta_dictate_buffer_s
    {
        uint64_t data_cursor;
        struct
        {
            int8_t unknown_data[YOTTA_DICTATE_UNKNOWN_SIZE];
        }
        data;
    }
    yotta_dictate_buffer_t;

    yotta_dictate_buffer_t * buffer =
        yotta_dictate_queue_recv_buffer(cmd_queue, yotta_dictate_buffer_t);

    uint64_t phony_cursor = 0; // Used to ignore data

    // If we have not received all the unknown data yet..
    if (buffer->data_cursor < cmd_queue->header.data_size)
    {
        // ..we try to get it
        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->data),
            &phony_cursor,
            &buffer->data
        );

        buffer->data_cursor += phony_cursor;

        if (op != 0) // We did not receive all the data
        {
            return;
        }
    }

    // Call the vtable's function linked to the daemon info reception
    cmd_queue->vtable->receive_unknown(cmd_queue->header.label, cmd_queue->header.data_size);

    // we clean up the tmp buffer
    buffer->data_cursor = 0;
    memset(&buffer->data, 0, sizeof(buffer->data));

    yotta_dictate_queue_finish(cmd_queue);
}

void
yotta_dictate_vtable_unknown_recv(yotta_dictate_label_t label, uint64_t data_size)
{
    fprintf(stderr, "Unknown frame of label %d and size %" PRIu64 "\n", label, data_size);
}
