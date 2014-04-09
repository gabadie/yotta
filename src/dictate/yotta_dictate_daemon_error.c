#include <stdio.h>
#include <string.h>

#include "yotta_dictate_daemon_error.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"

#define YOTTA_DICTATE_ERROR_MSG_MAX_SIZE 256

void
yotta_dictate_daemon_info_recv(
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
            char msg[YOTTA_DICTATE_ERROR_MSG_MAX_SIZE];
        }
        data;
    }
    yotta_dictate_buffer_t;

    yotta_dictate_buffer_t * buffer =
        yotta_dictate_queue_recv_buffer(cmd_queue, yotta_dictate_buffer_t);

    // If we have not received all the daemon info yet..
    if (buffer->data_cursor < sizeof(buffer->data))
    {
        // ..we try to get it
        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->data),
            &buffer->data_cursor,
            &buffer->data
        );

        if (op != 0) // We did not receive all the data
        {
            return;
        }
    }

    // Call the vtable's function linked to the daemon info reception
    cmd_queue->vtable->receive_daemon_error(buffer->data.msg);

    // we clean up the tmp buffer
    buffer->data_cursor = 0;
    memset(buffer->data.msg, 0, sizeof(buffer->data.msg));

    yotta_dictate_queue_finish(cmd_queue);

}

void
yotta_dictate_vtable_daemon_info_recv(char const * msg)
{
    fprintf(stderr, "Daemon error: %s\n", msg);
}
