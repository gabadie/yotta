
#include "yotta_whisper_command.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_addr_translation.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../massive/yotta_dispatch.private.h"
#include "../threading/yotta_thread.h"


/*
 * @infos: defines a command order command
 */
typedef struct
yotta_whisper_command_order_cmd_s
{
    // the TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        yotta_rel_addr_t function_rel_addr;
        uint64_t param_size;
        uint64_t sync_finished;
        uint64_t group_id;
        uint64_t group_count;
        uint64_t global_offset;
        uint64_t global_count;
    } __attribute__((packed))
    header;

    // the parameter
    uint64_t param_cursor;
    void const * param;

    // the sending_sync
    yotta_sync_t * sync_sent;
}
yotta_whisper_command_order_cmd_t;

/*
 * @infos: defines a command feedback command
 */
typedef struct
yotta_whisper_command_feedback_cmd_s
{
    // the TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        uint64_t sync_finished;
    } __attribute__((packed))
    header;

    // the user parameter
    void * param;

    // the thread entry point
    yotta_whisper_command_entry_t thread_entry;

    // the thread
    yotta_thread_t thread;

    // the thread's global pool
    yotta_global_thread_pool_t thread_global_pool;

    // the TCP queue to send back the feedback when the thread has finished
    yotta_whisper_queue_t * queue;
}
yotta_whisper_command_feedback_cmd_t;

/*
 * @infos: defines a command feedback's thread
 */
static
void *
yotta_whisper_command_thread(yotta_whisper_command_feedback_cmd_t * cmd)
{
    yotta_assert(cmd != 0);

    /*
     * Sets the thread's global pool
     */
    yotta_thread_global_pool = &cmd->thread_global_pool;

    /*
     * We call the thread entry with user parameters
     */
    cmd->thread_entry(cmd->param);

    /*
     * We release user parameters if necessary
     */
    if (cmd->param != 0)
    {
        yotta_free(cmd->param);
        cmd->param = 0;
    }

    /*
     * We appends the feedback command to the selected TCP queue
     */
    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd->queue, (yotta_tcp_cmd_t *) cmd);

    return 0;
}

/*
 * @infos: yotta_whisper_command_{order,feedback}_cmd_t's release event
 */
static
void
yotta_whisper_command_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != 0);

    /*
     * This is actually freeing yotta_whisper_command_feedback_cmd_t too, but
     * we don't need to free the yotta_whisper_command_feedback_cmd_t::param
     * because it will be already done before the command has been appended to
     * the TCP queue calling back this function.
     */
    yotta_free(cmd);
}

/*
 * @infos: yotta_whisper_command_order_cmd_t's send event
 */
static
void
yotta_whisper_command_order_send(yotta_whisper_command_order_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        /*
         * sends command's header
         */

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

    if (cmd->header.param_size != 0)
    {
        /*
         * sends command's parameter if one
         */

        uint64_t op = yotta_tcp_cmd_send(
            (yotta_tcp_cmd_t *) cmd,
            cmd->header.param_size,
            &cmd->param_cursor,
            cmd->param
        );

        if (op != 0)
        {
            return;
        }
    }

    /*
     * We have finished to send the command order, then we post the sending
     * sync object and destroy the current command
     */
    yotta_sync_post(cmd->sync_sent);

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

/*
 * @infos: yotta_whisper_command_feedback_cmd_t's send event
 */
static
void
yotta_whisper_command_feedback_send(yotta_whisper_command_feedback_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    if (cmd->header_cursor != sizeof(cmd->header))
    {
        /*
         * sends command feedback's header
         */

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

    /*
     * We have finished to send the command order, then we destroy the current
     * command
     */
    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

void
yotta_whisper_command_order_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != 0);

    /*
     * Defines the receiving buffer
     */
    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_cursor;
        struct
        {
            yotta_rel_addr_t function_rel_addr;
            uint64_t param_size;
            uint64_t sync_finished;
            uint64_t group_id;
            uint64_t group_count;
            uint64_t global_offset;
            uint64_t global_count;
        }
        header;

        uint64_t param_cursor;
        void * param;
    }
    yotta_whisper_buffer_t;

    /*
     * Gets the command order's receiving buffer
     */
    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    if (buffer->header_cursor < sizeof(buffer->header))
    {
        /*
         * we are receiving the command order's header
         */

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->header),
            &buffer->header_cursor,
            &buffer->header
        );

        if (op != 0)
        {
            return;
        }

        if (buffer->header.param_size != 0)
        {
            buffer->param = yotta_alloc(buffer->header.param_size);
        }
    }

    if (buffer->header.param_size != 0)
    {
        /*
         * we are receiving the command order's parameter if one
         */

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            buffer->header.param_size,
            &buffer->param_cursor,
            buffer->param
        );

        if (op != 0)
        {
            return;
        }
    }

    {
        /*
         * we create the command feedback command and launch its thread
         */

        yotta_whisper_command_feedback_cmd_t * cmd = yotta_alloc_s(yotta_whisper_command_feedback_cmd_t);

        yotta_dirty_s(cmd);

        yotta_tcp_cmd_init(cmd);
        yotta_tcp_cmd_set_send(cmd, yotta_whisper_command_feedback_send);
        yotta_tcp_cmd_set_release(cmd, yotta_whisper_command_release);

        cmd->header_cursor = 0;
        cmd->header.label = YOTTA_WHISPER_COMMAND_FEEDBACK;
        cmd->header.sync_finished = buffer->header.sync_finished;
        cmd->param = buffer->param;
        cmd->thread_global_pool.group_id = buffer->header.group_id;
        cmd->thread_global_pool.group_count = buffer->header.group_count;
        cmd->thread_global_pool.global_offset = buffer->header.global_offset;
        cmd->thread_global_pool.global_count = buffer->header.global_count;
        cmd->thread_entry =
            (yotta_whisper_command_entry_t) yotta_address_relative_to_absolute(buffer->header.function_rel_addr);
        cmd->queue = cmd_queue;

        if (yotta_thread_create(&cmd->thread, (void *) yotta_whisper_command_thread, (void *) cmd) != 0)
        {
            if (cmd->param)
            {
                yotta_free(cmd->param);
            }

            yotta_tcp_cmd_release(cmd);
        }
    }

    /*
     * we clean up the tmp buffer
     */
    buffer->header_cursor = 0;
    buffer->header.function_rel_addr = 0;
    buffer->header.param_size = 0;
    buffer->header.sync_finished = 0;
    buffer->header.group_id = 0;
    buffer->header.group_count = 0;
    buffer->header.global_offset = 0;
    buffer->header.global_count = 0;
    buffer->param_cursor = 0;
    buffer->param = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

void
yotta_whisper_command_feedback_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != 0);

    /*
     * Defines the receiving buffer
     */
    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_cursor;
        struct
        {
            uint64_t sync_finished;
        }
        header;
    }
    yotta_whisper_buffer_t;

    /*
     * Gets the command feedback's receiving buffer
     */
    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    if (buffer->header_cursor < sizeof(buffer->header))
    {
        /*
         * we are receiving the comand feedback's header
         */

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->header),
            &buffer->header_cursor,
            &buffer->header
        );

        if (op != 0)
        {
            return;
        }
    }

    /*
     * we post the feedback in the sync_finished
     */
    yotta_assert(buffer->header.sync_finished != 0);
    yotta_sync_post((yotta_sync_t *) buffer->header.sync_finished);

    // we clean up the tmp buffer
    buffer->header_cursor = 0;
    buffer->header.sync_finished = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

void
yotta_whisper_command(
    yotta_whisper_queue_t * cmd_queue,
    yotta_whisper_command_entry_t function_addr,
    uint64_t param_size,
    void const * param,
    uint64_t group_id,
    uint64_t group_count,
    uint64_t global_offset,
    uint64_t global_count,
    yotta_sync_t * sync_sent,
    yotta_sync_t * sync_finished
)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(function_addr != 0);
    yotta_assert((param_size == 0 && param == 0) || (param_size != 0 && param != 0));
    yotta_assert(sync_sent != 0);
    yotta_assert(sync_finished != 0);

    /*
     * creates the command order command and appends it to the given whisper queue
     */
    yotta_whisper_command_order_cmd_t * cmd = yotta_alloc_s(yotta_whisper_command_order_cmd_t);

    yotta_dirty_s(cmd);

    yotta_tcp_cmd_init(cmd);
    yotta_tcp_cmd_set_send(cmd, yotta_whisper_command_order_send);
    yotta_tcp_cmd_set_release(cmd, yotta_whisper_command_release);

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_WHISPER_COMMAND_ORDER;
    cmd->header.function_rel_addr = yotta_address_absolute_to_relative((uint64_t) function_addr);
    cmd->header.param_size = param_size;
    cmd->header.sync_finished = (uint64_t) sync_finished;
    cmd->header.group_id = group_id;
    cmd->header.group_count = group_count;
    cmd->header.global_offset = global_offset;
    cmd->header.global_count = global_count;
    cmd->param_cursor = 0;
    cmd->param = param;
    cmd->sync_sent = sync_sent;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}
