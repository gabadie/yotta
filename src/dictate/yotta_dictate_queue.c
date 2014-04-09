#include <string.h>
#include <errno.h>

#include "yotta_dictate_queue.private.h"
#include "yotta_dictate_labels.private.h"
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

        yotta_dictate_label_t label = 0x0000;

        ssize_t label_size = yotta_tcp_recv(&cmd_queue->tcp_queue.socket_event.socket, &label, sizeof(label));

        if (label_size == sizeof(label))
        {
            uint64_t data_size = 0;
            ssize_t data_size_size = yotta_tcp_recv(
                &cmd_queue->tcp_queue.socket_event.socket,
                &data_size, sizeof(uint64_t)
            );

            if (data_size_size == 0)
            {
                // the socket has been closed properly on the other side

                yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);
                yotta_socket_event_release(cmd_queue);

                return;
            }
            else if (data_size_size == -1)
            {
                int32_t errno_recv = errno;

                if (errno_recv == EAGAIN)
                {
                    // no incomming information available
                    break;
                }
            }

            cmd_queue->data_size = data_size;

            switch (label)
            {
                case YOTTA_DICTATE_LABEL_DEAMON_INFO:
                {
                    /*cmd_queue->callback = yotta_dictate_daemon_info_recv;*/
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

            yotta_assert(cmd_queue->callback != NULL);

            continue;
        }
        else if (label_size == 0)
        {
            // the socket has been closed properly on the other side

            yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);
            yotta_socket_event_release(cmd_queue);

            return;
        }
        else if (label_size == -1)
        {
            int32_t errno_recv = errno;

            if (errno_recv == EAGAIN)
            {
                // no incomming information available
                break;
            }
        }

        yotta_crash_msg("receive error");
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

    memset(&cmd_queue->recv_buffer, 0, YOTTA_DICTATE_RECV_BUFFER_SIZE);

    cmd_queue->data_size = 0;

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
