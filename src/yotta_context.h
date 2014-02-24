#ifndef _YOTTA_CONTEXT
#define _YOTTA_CONTEXT

#include "yotta_prefix.h"
#include "socket/yotta_socket.h"

/*
 * @infos: store all context's information
 */
typedef
struct yotta_context_s
{
    // Port used for incoming connections
    uint16_t incoming_port;

    // Socket used for slave connections
    yotta_socket_t slave;

} yotta_context_t;


/*
 * @infos: init the yotta library
 *
 * @param <context>: the yotta context to initialize
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_context_init(yotta_context_t * context, uint16_t incoming_port);

/*
 * @infos: connection to the yotta's server
 *
 * @param <context>: the yotta context to initialize
 * @param <ip>: the yotta server's ip
 * @param <port>: thoe yotta server's port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
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
uint64_t
yotta_context_destroy(yotta_context_t * context);

#endif
