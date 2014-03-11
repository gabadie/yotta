#ifndef _YOTTAPRIVATE_WHISPER_SHARED_COUNTER
#define _YOTTAPRIVATE_WHISPER_SHARED_COUNTER

#include "yotta_whisper_queue.private.h"
#include "../massive/yotta_shared_counter.h"


/*
 * @infos: processes a shared counter request
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_shared_counter_request_recv(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: processes a shared counter anwser
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_shared_counter_answer_recv(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: send a shared counter request
 *
 * @param <cmd_queue>: the comamnd queue to send to the pull request to
 * @param <shared_counter>: the shared counter
 */
void
yotta_whisper_shared_counter(
    yotta_whisper_queue_t * cmd_queue,
    yotta_shared_counter_t * shared_counter
);

#endif
