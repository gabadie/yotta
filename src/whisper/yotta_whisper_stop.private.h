#ifndef _YOTTAPRIVATE_WHISPER_STOP
#define _YOTTAPRIVATE_WHISPER_STOP

#include "yotta_whisper_queue.private.h"
#include "../threading/yotta_sync.private.h"


/*
 * @infos: stop the distant process
 *
 * @param <cmd_queue>: the comamnd queue to send to the stop signal to
 */
void
yotta_whisper_stop(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: processes a stop
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_stop_recv(
    yotta_whisper_queue_t * cmd_queue
);

#endif
