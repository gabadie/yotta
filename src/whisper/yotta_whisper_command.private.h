#ifndef _YOTTAPRIVATE_WHISPER_COMMAND
#define _YOTTAPRIVATE_WHISPER_COMMAND

#include "yotta_whisper_queue.private.h"
#include "../massive/yotta_context.h"
#include "../threading/yotta_sync.private.h"


/*
 * @infos: defines yotta whisper command entry function pointer type
 *
 *  - void * param: the user param
 */
typedef yotta_massive_command_entry_t yotta_whisper_command_entry_t;

/*
 * @infos: launches a command by a sending a command order to the other side
 *
 * @param <cmd_queue>: the comamnd queue to send to the pull request to
 * @param <function_addr>: the absolute function address
 * @param <param_size>: the parameter's size
 * @param <param>: the parameter
 * @param <sync_sent>: the sending sync object
 * @param <sync_finished>: the finish sync object
 */
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
);

/*
 * @infos: processes a command feedback
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_command_feedback_recv(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: processes a command order
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_command_order_recv(
    yotta_whisper_queue_t * cmd_queue
);


#endif
