#ifndef _YOTTA_DAEMON
#define _YOTTA_DAEMON

#include "../yotta_prefix.h"
#include "yotta_massive_fwd.h"
#include "../dictate/yotta_dictate_queue.private.h"
#include "../whisper/yotta_whisper_queue.private.h"

/*
 * @infos Structure containing the information about a daemon
 */
struct
yotta_daemon_s
{
    yotta_dictate_queue_t dictate_queue;
    yotta_whisper_queue_t whisper_queue;

    // Number of available computers known by the daemon
    uint64_t available_computers;

    // Total number of available threads
    uint64_t available_threads;
};

#endif //_YOTTA_DAEMON
