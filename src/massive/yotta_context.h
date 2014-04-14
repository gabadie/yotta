#ifndef _YOTTA_CONTEXT
#define _YOTTA_CONTEXT

#include "../yotta_prefix.h"
#include "../socket/yotta_socket_thread.h"
#include "../whisper/yotta_whisper_master.h"
#include "yotta_massive_fwd.h"

/*
 * @infos: store all context's information
 */
struct
yotta_context_s
{
    // Whisper master
    yotta_whisper_master_t whisper_master;

    // Socket thread
    yotta_socket_thread_t worker_thread;

    // Daemons list
    yotta_daemon_t * daemons;
};


/*
 * @infos: init the yotta library
 *
 * @param <context>: the yotta context to initialize
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_context_init(yotta_context_t * context, uint16_t incoming_port);

/*
 * @infos: connection to a yotta deamon
 *
 * @param <context>: the yotta context to initialize
 * @param <ip>: the yotta server's ip
 * @param <port>: thoe yotta server's port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_context_connect(yotta_context_t * context, const char * ip, uint16_t port);

/*
 * @infos: destroy a yotta context
 *
 * @param <context>: the yotta context previously initialized
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_context_destroy(yotta_context_t * context);

#endif
