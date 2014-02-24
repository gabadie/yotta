#ifndef _YOTTAPRIVATE_TCP_CMD_QUEUE
#define _YOTTAPRIVATE_TCP_CMD_QUEUE

#include "yotta_socket_event.h"
#include "yotta_tcp_cmd.private.h"


/*
 * @infos: overloads yotta_socket_event_t
 */
typedef struct
yotta_tcp_cmd_queue_s
{
    // overloads yotta_socket_event_t
    yotta_socket_event_t socket_event;

    // the first and last TCP command in the queue
    yotta_tcp_cmd_t * queue_first;
    yotta_tcp_cmd_t * queue_last;
}
yotta_tcp_cmd_queue_t;

/*
 * @infos: inits TCP command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_tcp_cmd_queue_init(yotta_tcp_cmd_queue_t * cmd_queue);

/*
 * @infos: append a command in the queue
 *
 * @param <cmd_queue>: the command queue to append to
 * @param <cmd>: the command to append
 */
void
yotta_tcp_cmd_queue_append(yotta_tcp_cmd_queue_t * cmd_queue, yotta_tcp_cmd_t * cmd);

/*
 * @infos: cleans TCP command queue
 *
 * @param <cmd_queue>: the command queue to clean
 */
void
yotta_tcp_cmd_queue_destroy(yotta_tcp_cmd_queue_t * cmd_queue);


#endif
