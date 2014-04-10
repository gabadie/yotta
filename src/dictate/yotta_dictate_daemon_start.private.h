#ifndef _YOTTA_DICTATE_DAEMON_START
#define _YOTTA_DICTATE_DAEMON_START

#include "yotta_dictate_queue.private.h"
#include "../threading/yotta_sync.h"

/*
 * @infos: Start the process on the daemon
 *
 * @param <cmd_queue>: the command queue to send the binary
 * @param <data_size>: the data size
 * @param <data>: the data (binary) to send
 * @param <sync>: the sync object when finished
 */
void yotta_dictate_daemon_start(
    yotta_dictate_queue_t * cmd_queue,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync_finished
);

#endif //_YOTTA_DICTATE_DAEMON_START
