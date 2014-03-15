
#include "yotta_tcp.h"
#include "yotta_tcp_cmd.private.h"
#include "yotta_tcp_queue.private.h"
#include "../core/yotta_debug.h"


uint64_t
yotta_tcp_cmd_send(yotta_tcp_cmd_t * cmd, uint64_t buffer_size, uint64_t * buffer_cursor, void const * buffer)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->queue != 0);
    yotta_assert(buffer_size != 0);
    yotta_assert(buffer_cursor != 0);
    yotta_assert(buffer != 0);

    ssize_t buffer_sending = buffer_size - *buffer_cursor;

    ssize_t buffer_sent = yotta_tcp_send(
        (yotta_socket_t *) yotta_tcp_cmd_queue(cmd),
        ((uint8_t *) buffer) + *buffer_cursor,
        buffer_sending
    );

    if (buffer_sent == -1)
    {
        return 1;
    }

    *buffer_cursor += (uint64_t)buffer_sent;

    yotta_assert(*buffer_cursor <= buffer_size);

    return *buffer_cursor != buffer_size;
}

void
yotta_tcp_cmd_finish(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->queue != 0);
    yotta_assert(cmd->queue->queue_first == cmd);

    do // atomic command dequeuing
    {
        if (
            __sync_bool_compare_and_swap(
                &cmd->queue->queue_first,
                cmd,
                cmd->queue_next
            )
        )
        {
            break;
        }
    }
    while (1);

#ifdef YOTTA_ASSERT
    cmd->queue = 0;
#endif //YOTTA_ASSERT
}
