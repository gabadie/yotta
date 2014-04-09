#include <string.h>
#include <errno.h>

#include "yotta_dictate_queue.private.h"
#include "yotta_dictate_daemon_info.private.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_debug.h"
#include "../socket/yotta_tcp.h"
#include "../socket/yotta_socket_thread.h"


#ifdef YOTTA_DEBUG

/*
 * @infos: verify that the receive buffer has been cleaned up
 *
 * @param <cmd_queue>: the command queue to verify
 */
static
void
yotta_dictate_queue_assert_buffer(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    for (size_t i = 0; i < sizeof(cmd_queue->recv_buffer); i++)
    {
        yotta_assert(cmd_queue->recv_buffer[i] == 0);
    }
}

#endif //YOTTA_DEBUG


/*
 * Default vtable for yotta_dictate_queue_s
 */
static
yotta_dictate_vtable_t const
yotta_dictate_queue_default_vtable =
{
    yotta_dictate_vtable_daemon_info_recv
};

static
void
yotta_dictate_queue_recv(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != NULL);

    while (1)
    {
        if (cmd_queue->callback != NULL)
        {
            cmd_queue->callback(cmd_queue);

            if (cmd_queue->callback != NULL)
            {
                // the callback has not finished to receive all data
                break;
            }

#ifdef YOTTA_DEBUG
            yotta_dictate_queue_assert_buffer(cmd_queue);
#endif //YOTTA_DEBUG
        }

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(cmd_queue->header),
            &cmd_queue->header_cursor,
            &cmd_queue->header
        );

        if (op != 0)
        {
            return;
        }

        switch (cmd_queue->header.label)
        {
            case YOTTA_DICTATE_LABEL_DEAMON_INFO:
            {
                cmd_queue->callback = yotta_dictate_daemon_info_recv;
                break;
            }

            case YOTTA_DICTATE_LABEL_ERROR:
            {
                /*cmd_queue->callback = yotta_dictate_error_recv;*/
                break;
            }

            default:
            {
                /*cmd_queue->callback = yotta_dictate_unknown_label;*/
            }
        }
    }
}

static
void
yotta_dictate_queue_except(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_logger_error("yotta_dictate_queue_except: received a TCP socket exception -> releasing");

    yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);

    yotta_socket_event_release(cmd_queue);
}

void
yotta_dictate_queue_init(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(sizeof(yotta_dictate_label_t) == 2);

    yotta_dirty_offset(
        cmd_queue,
        sizeof(cmd_queue->tcp_queue),
        sizeof(*cmd_queue) - sizeof(cmd_queue->tcp_queue)
    );

    yotta_tcp_queue_init((yotta_tcp_queue_t *) cmd_queue);
    yotta_socket_event_set_recv(cmd_queue, yotta_dictate_queue_recv);
    yotta_socket_event_set_except(cmd_queue, yotta_dictate_queue_except);

    cmd_queue->callback = NULL;

    memset(&cmd_queue->header, 0, sizeof(cmd_queue->header));

    cmd_queue->header_cursor = 0;

    memset(&cmd_queue->recv_buffer, 0, YOTTA_DICTATE_RECV_BUFFER_SIZE);

    cmd_queue->vtable = &yotta_dictate_queue_default_vtable;
}

yotta_return_t
yotta_dictate_queue_connect(yotta_dictate_queue_t * cmd_queue, char const * ip, uint16_t port)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(ip != NULL);
    yotta_assert(port != 0);

    yotta_dirty_s(cmd_queue);

    if (yotta_tcp_socket_client((yotta_socket_t *) cmd_queue, ip, port) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_dictate_queue_init(cmd_queue);

    return YOTTA_SUCCESS;
}

void
yotta_dictate_queue_destroy(yotta_dictate_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != NULL);

    yotta_tcp_queue_destroy((yotta_tcp_queue_t *) cmd_queue);
}
