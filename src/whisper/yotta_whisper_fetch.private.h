#ifndef _YOTTAPRIVATE_WHISPER_FETCH
#define _YOTTAPRIVATE_WHISPER_FETCH

#include "yotta_whisper_queue.private.h"


/*
 * @infos: processes a fetch request
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_fetch_request_recv(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: processes a fetch anwser
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_fetch_answer_recv(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: send a fetches request
 *
 * @param <cmd_queue>: the comamnd queue to send to the pull request to
 * @param <master_address>: absolute address in the master memory
 * @param <data_size>: the data size
 * @param <data_dest>: the data destination
 */
void
yotta_whisper_fetch(
    yotta_whisper_queue_t * cmd_queue,
    uint64_t master_address,
    uint64_t data_size,
    void * data_dest
);

#endif
