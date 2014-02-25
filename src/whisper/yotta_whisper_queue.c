
#include <string.h>

#include "yotta_whisper_queue.private.h"
#include "yotta_whisper_labels.private.h"
#include "yotta_whisper_push.private.h"
#include "../socket/yotta_tcp.h"
#include "../socket/yotta_socket_thread.h"
#include "../yotta_logger.private.h"
#include "../yotta_debug.h"


static
void
yotta_whisper_queue_recv(yotta_whisper_queue_t * cmd_queue)
{
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

        uint16_t label = 0;

        ssize_t label_size = yotta_tcp_recv(&cmd_queue->tcp_queue.socket_event.socket, &label, sizeof(label));

        if (label_size == sizeof(label))
        {
            switch (label)
            {
                case YOTTA_WHISPER_MEM_PUSH:
                    cmd_queue->callback = yotta_whisper_entry_push;
                    break;

                default:
                    yotta_crash_msg("unknown whisper label");
            }

            continue;
        }
        else if (label_size == 0)
        {
            // the socket has been closed properly on the other side

            yotta_socket_thread_unlisten(cmd_queue->tcp_queue.socket_event.socket_thread, (yotta_socket_event_t *) cmd_queue);
            yotta_socket_event_release(cmd_queue);

            return;
        }
        else if (label_size == 1)
        {
            yotta_crash_msg("unsupported label receiving");
        }

        yotta_crash_msg("select error");
    }
}

static
void
yotta_whisper_queue_except(yotta_whisper_queue_t * cmd_queue)
{
    yotta_logger_error("yotta_whisper_queue_except: received a TCP socket exception -> releasing");

    yotta_socket_event_release(cmd_queue);
}

void
yotta_whisper_queue_init(yotta_whisper_queue_t * cmd_queue)
{
    yotta_tcp_queue_init((yotta_tcp_queue_t *) cmd_queue);
    yotta_socket_event_set_recv(cmd_queue, yotta_whisper_queue_recv);
    yotta_socket_event_set_except(cmd_queue, yotta_whisper_queue_except);

    cmd_queue->callback = 0;

    memset(&cmd_queue->recv_buffer, 0, YOTTA_WHISPER_RECV_BUFFER_SIZE);
}

void
yotta_whisper_queue_destroy(yotta_whisper_queue_t * cmd_queue)
{
    yotta_tcp_queue_destroy((yotta_tcp_queue_t *) cmd_queue);
}