
#include "yotta_tcp_queue.private.h"
#include "yotta_socket_thread.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"


static
void
yotta_tcp_queue_pop_stack(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_tcp_cmd_t * queue_stack = cmd_queue->queue_stack;

    if (queue_stack == 0)
    {
        return;
    }

    do // atomic commands stack fetching
    {
        queue_stack = cmd_queue->queue_stack;
    }
    while (!__sync_bool_compare_and_swap(&cmd_queue->queue_stack, queue_stack, 0));

    // sets the new commands queue's end as the first of the commands stack
    cmd_queue->queue_last = queue_stack;

    // reverses queue_stack into queue_append
    yotta_tcp_cmd_t * queue_append = 0;

    while (queue_stack)
    {
        yotta_tcp_cmd_t * cmd = queue_stack;

        queue_stack = queue_stack->queue_next;
        cmd->queue_next = queue_append;
        queue_append = cmd;
    }

    // appends queue_append to the back of comamnds queue
    if (cmd_queue->queue_first == 0)
    {
        cmd_queue->queue_first = queue_append;
    }
    else
    {
        cmd_queue->queue_last->queue_next = queue_append;
    }
}

static
void
yotta_tcp_queue_send(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_tcp_queue_pop_stack(cmd_queue);

    yotta_tcp_cmd_t * cmd = cmd_queue->queue_first;

    if (cmd == 0)
    {
        yotta_socket_event_set_send(cmd_queue, 0);
        return;
    }

    yotta_assert(cmd->send_event != 0);

    cmd->send_event(cmd);
}

void
yotta_tcp_queue_init(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_socket_nonblock((yotta_socket_t *) cmd_queue);

    yotta_socket_event_init(cmd_queue);
    yotta_socket_event_set_send(cmd_queue, 0);

    cmd_queue->queue_first = 0;
}

void
yotta_tcp_queue_append(yotta_tcp_queue_t * cmd_queue, yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(cmd != 0);
    yotta_assert(cmd->queue == 0);

    cmd->queue = cmd_queue;

    do // atomic commands stack pushing
    {
        cmd->queue_next = cmd_queue->queue_stack;
    }
    while (!__sync_bool_compare_and_swap(&cmd_queue->queue_stack, cmd->queue_next, cmd));

    yotta_socket_event_set_send(cmd_queue, yotta_tcp_queue_send);
}

static
void
yotta_tcp_queue_finish_send(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(yotta_tcp_cmd_queue(cmd));

    yotta_socket_event_t * socket_event = (yotta_socket_event_t * )yotta_tcp_cmd_queue(cmd);

    yotta_socket_event_unlisten(socket_event);

    yotta_socket_event_release(socket_event);
}

static
void
yotta_tcp_queue_finish_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != 0);

    yotta_free(cmd);
}

void
yotta_tcp_queue_finish(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(cmd_queue->socket_event.socket_thread != 0);

    yotta_tcp_cmd_t * cmd = yotta_alloc_s(yotta_tcp_cmd_t);

    yotta_tcp_cmd_init(cmd);
    yotta_tcp_cmd_set_send(cmd, yotta_tcp_queue_finish_send);
    yotta_tcp_cmd_set_release(cmd, yotta_tcp_queue_finish_release);

    yotta_tcp_queue_append(cmd_queue, cmd);
}

void
yotta_tcp_queue_destroy(yotta_tcp_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    while (cmd_queue->queue_first)
    {
        yotta_tcp_cmd_t * cmd = cmd_queue->queue_first;

        cmd_queue->queue_first = cmd->queue_next;

        cmd->release_event(cmd);
    }

    yotta_socket_event_destroy(cmd_queue);
}
