
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../slave/yotta_slave_context.private.h"


/*
 * @infos: defines a stop command
 */
typedef struct
yotta_whisper_stop_cmd_s
{
    // the TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
    } __attribute__((packed))
    header;
}
yotta_whisper_stop_cmd_t;


/*
 * @infos: yotta_whisper_stop_cmd_t's release event
 */
static
void
yotta_whisper_stop_release(yotta_whisper_stop_cmd_t * cmd)
{
    yotta_free(cmd);
}

/*
 * @infos: yotta_whisper_stop_cmd_t's send event
 */
static
void
yotta_whisper_stop_send(yotta_whisper_stop_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);
    yotta_assert(cmd->abstract_cmd.queue != NULL);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        // send push's header

        uint64_t op = yotta_tcp_cmd_send(
            (yotta_tcp_cmd_t *) cmd,
            sizeof(cmd->header),
            &cmd->header_cursor,
            &cmd->header
        );

        if (op != 0)
        {
            return;
        }
    }

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

void
yotta_whisper_stop_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != NULL);

    /**
     * We are receiving a stop commend that has nothing more than the label that
     * has already been processed to find out this callback.
     *
     * yotta_tcp_queue_finish() will push the finish tcp command and the queue's
     * tail, calling the whisper's release_event.
     */

    yotta_tcp_queue_finish(&cmd_queue->tcp_queue);

    yotta_whisper_queue_finish(cmd_queue);
}

void
yotta_whisper_stop(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(yotta_slave_context == NULL); // must not be call in the master process

    /*
     * Creates the YOTTA_WHISPER_STOP command
     */
    yotta_whisper_stop_cmd_t * cmd = yotta_alloc_s(yotta_whisper_stop_cmd_t);

    yotta_dirty_s(cmd);

    yotta_tcp_cmd_init(cmd);
    yotta_tcp_cmd_set_send(cmd, yotta_whisper_stop_send);
    yotta_tcp_cmd_set_release(cmd, yotta_whisper_stop_release);

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_WHISPER_STOP;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}
