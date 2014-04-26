
#include "yotta_whisper_push.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../slave/yotta_slave_context.private.h"


/*
 * @infos: yotta_whisper_stop_cmd_t's release event
 */
static
void
yotta_whisper_stop_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    yotta_tcp_cmd_destroy(cmd);
    yotta_free(cmd);
}

/*
 * @infos: yotta_whisper_stop_cmd_t's send event
 */
static
void
yotta_whisper_stop_send(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);
    yotta_assert(cmd->queue != NULL);

    static
    yotta_whisper_label_t const label = YOTTA_WHISPER_STOP;
    uint64_t label_cursor = 0;

    yotta_assert(sizeof(label) == 1);

    // Streams stop's label
    yotta_tcp_cmd_stream_unique(
        (yotta_tcp_cmd_t *) cmd,
        sizeof(label),
        &label_cursor,
        &label
    );

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

static
yotta_tcp_cmd_vtable_t const
yotta_whisper_stop_vtable = {
    (yotta_tcp_cmd_entry_t) yotta_whisper_stop_release,
    (yotta_tcp_cmd_entry_t) yotta_whisper_stop_send
};

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
    yotta_tcp_cmd_t * cmd = yotta_alloc_s(yotta_tcp_cmd_t);

    yotta_dirty_s(cmd);
    yotta_tcp_cmd_init(cmd, &yotta_whisper_stop_vtable);

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, cmd);
}
