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

    // the deamon's context
    yotta_context_t * context;

    // Number of available computers known by the daemon
    uint64_t available_computers;

    // Total number of available threads
    uint64_t available_threads;

    // the deamon status
    uint64_t status;
};


#define YOTTA_DAEMON_DICTATE_READY 0x1
#define YOTTA_DAEMON_WHISPER_READY 0x2


/*
 * @infos: init the given deamon
 *
 * @param <deamon>: the yotta deamon to initialize
 * @param <ip>: the yotta deamon's ip address
 * @param <port>: the yotta deamon's listening port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_daemon_init(yotta_daemon_t * daemon, yotta_context_t * context, char const * ip, uint16_t port);

/*
 * @infos: destroy the given deamon
 *
 * @param <deamon>: the yotta deamon to destroy
 */
void
yotta_daemon_destroy(yotta_daemon_t * daemon);


#endif //_YOTTA_DAEMON
