#ifndef _YOTTA_DICTATE_SEND_MEMORY_BLOCK
#define _YOTTA_DICTATE_SEND_MEMORY_BLOCK

#include "yotta_dictate_queue.private.h"
#include "yotta_dictate_labels.private.h"
#include "../threading/yotta_sync.h"

/*
 * @infos: Send a memory block
 *
 * @param <cmd_queue>: the command queue used to send the memory block
 * @param <label>: label of the frame
 * @param <data_size>: the data size
 * @param <data>: the data to send
 * @param <sync>: the sync object when finished
 */
void yotta_dictate_send_memory_block(
    yotta_dictate_queue_t * cmd_queue,
    yotta_dictate_label_t label,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync_finished
);

#endif //_YOTTA_DICTATE_SEND_MEMORY_BLOCK
