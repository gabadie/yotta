#ifndef _YOTTA_DICTATE_BINARY_SEND
#define _YOTTA_DICTATE_BINARY_SEND

#include "yotta_dictate_queue.private.h"
#include "../threading/yotta_sync.h"

/*
 * @infos: Sends the binary (<data>) to a slave
 *
 * @param <cmd_queue>: the command queue to send the binary
 * @param <executable_path>: the path of the binary to send
 * @param <sync>: the sync object when finished
 */
uint64_t
yotta_dictate_binary(
    yotta_dictate_queue_t * cmd_queue,
    char const * executable_path,
    yotta_sync_t * sync_finished
);

#endif //_YOTTA_DICTATE_BINARY_SEND
