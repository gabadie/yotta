#ifndef _YOTTA_DICTATE_BINARY_SEND
#define _YOTTA_DICTATE_BINARY_SEND

#include "../massive/yotta_context.h"
#include "yotta_dictate_queue.private.h"
#include "../threading/yotta_sync.h"

/*
 *
 */
void yotta_dictate_binary(
    yotta_dictate_queue_t * cmd_queue,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync_finished
);

#endif //_YOTTA_DICTATE_BINARY_SEND
