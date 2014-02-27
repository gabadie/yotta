
#include "yotta_tcp_cmd.private.h"
#include "yotta_tcp_queue.private.h"
#include "../core/yotta_debug.h"


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

#ifdef YOTTA_DEBUG
    cmd->queue = 0;
#endif // YOTTA_DEBUG
}
