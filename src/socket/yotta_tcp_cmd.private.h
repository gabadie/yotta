#ifndef _YOTTAPRIVATE_TCP_CMD
#define _YOTTAPRIVATE_TCP_CMD

#include "yotta_socket_event.h"


/*
 * @infos; forward declaration of yotta_tcp_cmd_t
 */
typedef struct
yotta_tcp_cmd_s
yotta_tcp_cmd_t;

/*
 * @infos; forward declaration of yotta_tcp_queue_t
 */
typedef struct
yotta_tcp_queue_s
yotta_tcp_queue_t;

/*
 * @infos: defines yotta tcp command entry point function pointer type
 *
 * The tcp command entry received 2 parameters:
 *  - yotta_tcp_cmd_t cmd: the tcp_cmd
 *  - yotta_tcp_queue_t cmd_queue: the parrent cmd_queue
 */
typedef void (*yotta_tcp_entry_t)(yotta_tcp_cmd_t *, yotta_tcp_queue_t *);

/*
 * @infos: defines yotta tcp command entry point function pointer type
 *
 * The tcp command entry received 1 parameters:
 *  - yotta_tcp_cmd_t cmd: the tcp_cmd
 */
typedef void (*yotta_tcp_cmd_release_t)(yotta_tcp_cmd_t *);

/*
 * @infos: defines a TCP command to be inherited
 */
struct
yotta_tcp_cmd_s
{
    // the send event entry point
    yotta_tcp_entry_t send_event;

    // the release the tcp_cmd
    yotta_tcp_cmd_release_t release_event;

    // the next TCP command in the queue
    yotta_tcp_cmd_t * queue_next;
};

/*
 * @infos: sets TCP command's event entries
 *
 * @param <socket_event>: the tcp command event
 * @param <function_ptr>: the function pointer
 */
#define yotta_tcp_cmd_set_send(socket_event,function_ptr) \
    ((yotta_tcp_cmd_t *) (socket_event))->send_event = (yotta_tcp_entry_t)(function_ptr)

#define yotta_tcp_cmd_set_release(socket_event,function_ptr) \
    ((yotta_tcp_cmd_t *) (socket_event))->release_event = (yotta_tcp_cmd_release_t)(function_ptr)


#endif
