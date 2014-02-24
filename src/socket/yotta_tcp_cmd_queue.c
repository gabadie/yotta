
#include "yotta_tcp_cmd_queue.private.h"
#include "../yotta_debug.h"


static
void
yotta_tcp_cmd_queue_send(yotta_tcp_cmd_queue_t * cmd_queue)
{
    yotta_tcp_cmd_t * cmd = cmd_queue->queue_first;

    yotta_assert(cmd != 0);
    yotta_assert(cmd->send_event != 0);

    cmd->send_event(cmd, cmd_queue);
}

void
yotta_tcp_cmd_queue_init(yotta_tcp_cmd_queue_t * cmd_queue)
{
    yotta_socket_event_send(cmd_queue, yotta_tcp_cmd_queue_send);

    cmd_queue->queue_first = 0;

#ifdef YOTTA_DEBUG
    cmd_queue->queue_last = 0;
#endif // YOTTA_DEBUG
}

void
yotta_tcp_cmd_queue_append(yotta_tcp_cmd_queue_t * cmd_queue, yotta_tcp_cmd_t * cmd)
{
    cmd->queue_next = 0;

    if (cmd_queue->queue_first == 0)
    {
        cmd_queue->queue_first = cmd;
    }
    else
    {
        cmd_queue->queue_last->queue_next = cmd;
    }

    cmd_queue->queue_last = cmd;
}

void
yotta_tcp_cmd_queue_destroy(yotta_tcp_cmd_queue_t * cmd_queue)
{
    while (cmd_queue->queue_first)
    {
        yotta_tcp_cmd_t * cmd = cmd_queue->queue_first;

        cmd_queue->queue_first = cmd->queue_next;

        cmd->release_event(cmd);
    }

    yotta_socket_event_destroy(cmd_queue);
}
