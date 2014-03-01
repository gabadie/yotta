#ifndef _YOTTAPRIVATE_SLAVE_CONTEXT
#define _YOTTAPRIVATE_SLAVE_CONTEXT

#include "yotta_slave.private.h"
#include "../socket/yotta_socket_thread.h"
#include "../whisper/yotta_whisper_queue.private.h"


/*
 * @infos: yotta slave context
 */
typedef struct
yotta_slave_context_s
{
    // the slave sockets' thread
    yotta_socket_thread_t thread;

    // the whisper comamnd queue
    yotta_whisper_queue_t queue;
}
yotta_slave_context_t;


/*
 * @infos: the global slave context
 *      instantiated in yotta_slave_context.c
 */
extern
yotta_slave_context_t * yotta_slave_context;


/*
 * @infos: Slave launching called if the --yotta parameters is given to yotta_init()
 *
 * @param <out_parameters>: yotta's slave parameters parsed by yotta_slave_parse_parameters()
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
uint64_t
yotta_slave_context_main(yotta_slave_parameters_t const * out_parameters);

#endif
