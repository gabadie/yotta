#ifndef _YOTTAPRIVATE_TCP_QUEUE
#define _YOTTAPRIVATE_TCP_QUEUE

#include "yotta_socket_event.h"
#include "yotta_tcp_cmd.private.h"


/*
 * @infos: overloads yotta_socket_event_t
 */
typedef struct
yotta_tcp_queue_s
{
    // overloads yotta_socket_event_t
    yotta_socket_event_t socket_event;

    // the first and last TCP command in the queue
    yotta_tcp_cmd_t * queue_first;
    yotta_tcp_cmd_t * queue_last;

    // the command queue stack entry
    yotta_tcp_cmd_t * queue_stack;
}
yotta_tcp_queue_t;

/*
 * @infos: inits TCP command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_tcp_queue_init(yotta_tcp_queue_t * cmd_queue);

/*
 * @threadsafe
 * @infos: appends a command in the queue
 *
 * @param <cmd_queue>: the command queue to append to
 * @param <cmd>: the command to append
 */
void
yotta_tcp_queue_append(yotta_tcp_queue_t * cmd_queue, yotta_tcp_cmd_t * cmd);

/*
 * @infos: receives the buffer through the TCP command's queue
 *
 * @param <cmd_queue>: the TCP command queue
 * @param <buffer_size>: the buffer's size to send
 * @param <buffer_cursor>: the buffer's sending cursor
 * @param <buffer>: the buffer to send
 *
 * @returns
 *  - <0> when *buffer_cursor == buffer_size
 *  - <1> if *buffer_cursor < buffer_size
 */
uint64_t
yotta_tcp_queue_recv(yotta_tcp_queue_t * cmd_queue, uint64_t buffer_size, uint64_t * buffer_cursor, void * buffer);

/*
 * @infos: async finish the TCP command queue after processing all comamnds
 *
 * @param <cmd_queue>: the command queue to close
 */
void
yotta_tcp_queue_finish(yotta_tcp_queue_t * cmd_queue);

/*
 * @infos: cleans TCP command queue
 *
 * @param <cmd_queue>: the command queue to clean
 */
void
yotta_tcp_queue_destroy(yotta_tcp_queue_t * cmd_queue);


#endif
