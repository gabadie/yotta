
#include <string.h>

#include "yotta_whisper_queue.private.h"
#include "../yotta_logger.private.h"


static
void
yotta_whisper_queue_recv(yotta_whisper_queue_t * cmd_queue)
{
    (void) cmd_queue;
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
    yotta_tcp_cmd_queue_init((yotta_tcp_cmd_queue_t *) cmd_queue);
    yotta_socket_event_set_recv(cmd_queue, yotta_whisper_queue_recv);
    yotta_socket_event_set_except(cmd_queue, yotta_whisper_queue_except);

    cmd_queue->callback = 0;

    memset(&cmd_queue->recv_buffer, 0, YOTTA_WHISPER_RECV_BUFFER_SIZE);
}

void
yotta_whisper_queue_destroy(yotta_whisper_queue_t * cmd_queue)
{
    yotta_tcp_cmd_queue_destroy((yotta_tcp_cmd_queue_t *) cmd_queue);
}
