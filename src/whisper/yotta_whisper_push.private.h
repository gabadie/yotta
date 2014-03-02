#ifndef _YOTTAPRIVATE_WHISPER_PUSH
#define _YOTTAPRIVATE_WHISPER_PUSH

#include "yotta_whisper_queue.private.h"
#include "../threading/yotta_sync.private.h"


/*
 * @infos: pushes <data> in the <socket>
 *
 * @param <cmd_queue>: the comamnd queue to send to the pull request to
 * @param <master_address>: absolute address in the master memory
 * @param <data_size>: the data size
 * @param <data>: the data to push
 * @param <sync>: the sync object when finished
 */
void
yotta_whisper_push(
    yotta_whisper_queue_t * cmd_queue,
    uint64_t master_address,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync
);

/*
 * @infos: processes a push
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_push_master_recv(
    yotta_whisper_queue_t * cmd_queue
);

#endif
