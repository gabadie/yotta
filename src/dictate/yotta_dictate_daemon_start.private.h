#ifndef _YOTTA_DICTATE_DAEMON_START
#define _YOTTA_DICTATE_DAEMON_START

#include "yotta_dictate_queue.private.h"
#include "yotta_dictate_send_memory_block.private.h"
#include "../threading/yotta_sync.h"

/*
 * @infos: Start the process on the daemon
 *
 * @param <cmd_queue>: the command queue to send the binary
 * @param <port>: the listening port of the whisper connection
 * @param <sync_finished>: the sync object when finished
 */
#define yotta_dictate_daemon_start(cmd_queue, port, sync_finished) \
{ \
    uint16_t p = (port); \
    yotta_dictate_send_memory_block( \
        cmd_queue, \
        YOTTA_DICTATE_LABEL_DAEMON_START, \
        2, \
        &p, \
        sync_finished \
    ); \
}


#endif //_YOTTA_DICTATE_DAEMON_START
