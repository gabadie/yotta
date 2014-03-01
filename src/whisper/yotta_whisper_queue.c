
#include <string.h>
#include <errno.h>

#include "yotta_whisper_queue.private.h"
#include "yotta_whisper_labels.private.h"
#include "yotta_whisper_command.private.h"
#include "yotta_whisper_fetch.private.h"
#include "yotta_whisper_push.private.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_debug.h"
#include "../socket/yotta_tcp.h"
#include "../socket/yotta_socket_thread.h"


/*
 * @infos: groups all whisper protocol labels' entries
 *
 * @important: must the order of yotta_whisper_label_t (yotta_whisper_labels.private.h)
 */
static
yotta_whisper_recv_t const
yotta_whisper_label_entries[YOTTA_WHISPER_LABELS_COUNT] =
{
    (yotta_whisper_recv_t) yotta_whisper_command_feedback_recv,
    (yotta_whisper_recv_t) yotta_whisper_command_order_recv,
    (yotta_whisper_recv_t) yotta_whisper_fetch_answer_recv,
    (yotta_whisper_recv_t) yotta_whisper_fetch_request_recv,
    (yotta_whisper_recv_t) yotta_whisper_push_master_recv
};

static
void
yotta_whisper_queue_recv(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    while (1)
    {
        if (cmd_queue->callback != 0)
        {
            cmd_queue->callback(cmd_queue);

            if (cmd_queue->callback != 0)
            {
                // the callback has not finished to receive all data
                break;
            }
        }

        yotta_whisper_label_t label = 0;

        ssize_t label_size = yotta_tcp_recv(&cmd_queue->tcp_queue.socket_event.socket, &label, sizeof(label));

        if (label_size == sizeof(label))
        {
            yotta_assert(label < YOTTA_WHISPER_LABELS_COUNT);
            yotta_assert(yotta_whisper_label_entries[label] != 0);

            cmd_queue->callback = yotta_whisper_label_entries[label];

            yotta_assert(cmd_queue->callback != 0);

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
yotta_whisper_queue_except(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_logger_error("yotta_whisper_queue_except: received a TCP socket exception -> releasing");

    yotta_socket_event_unlisten((yotta_socket_event_t *) cmd_queue);

    yotta_socket_event_release(cmd_queue);
}

void
yotta_whisper_queue_init(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(YOTTA_WHISPER_LABELS_COUNT <= sizeof(yotta_whisper_label_t) * 8);
    yotta_assert(sizeof(yotta_whisper_label_t) == 1);

    yotta_dirty_offset(
        cmd_queue,
        sizeof(cmd_queue->tcp_queue),
        sizeof(*cmd_queue) - sizeof(cmd_queue->tcp_queue)
    );

    yotta_tcp_queue_init((yotta_tcp_queue_t *) cmd_queue);
    yotta_socket_event_set_recv(cmd_queue, yotta_whisper_queue_recv);
    yotta_socket_event_set_except(cmd_queue, yotta_whisper_queue_except);

    cmd_queue->callback = 0;

    memset(&cmd_queue->recv_buffer, 0, YOTTA_WHISPER_RECV_BUFFER_SIZE);
}

uint64_t
yotta_whisper_queue_connect(yotta_whisper_queue_t * cmd_queue, char const * ip, uint16_t port)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(ip != 0);
    yotta_assert(port != 0);

    yotta_dirty_s(cmd_queue);

    if (yotta_tcp_socket_client((yotta_socket_t *) cmd_queue, ip, port) != 0)
    {
        return -1;
    }

    yotta_whisper_queue_init(cmd_queue);

    return 0;
}

void
yotta_whisper_queue_destroy(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_tcp_queue_destroy((yotta_tcp_queue_t *) cmd_queue);
}
