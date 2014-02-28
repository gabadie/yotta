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
 */
typedef void (*yotta_tcp_cmd_entry_t)(yotta_tcp_cmd_t *);

/*
 * @infos: defines a TCP command to be inherited
 */
struct
yotta_tcp_cmd_s
{
    // the send event entry point
    yotta_tcp_cmd_entry_t send_event;

    // the release the tcp_cmd
    yotta_tcp_cmd_entry_t release_event;

    // the next TCP command in the queue
    yotta_tcp_cmd_t * queue_next;

    // the parent TCP command queue
    yotta_tcp_queue_t * queue;
};

/*
 * @infos: sets TCP command's event entries
 *
 * @param <cmd>: the tcp command
 * @param <function_ptr>: the function pointer
 */
#define yotta_tcp_cmd_set_send(cmd,function_ptr) \
    ((yotta_tcp_cmd_t *) (cmd))->send_event = (yotta_tcp_cmd_entry_t)(function_ptr)

#define yotta_tcp_cmd_set_release(cmd,function_ptr) \
    ((yotta_tcp_cmd_t *) (cmd))->release_event = (yotta_tcp_cmd_entry_t)(function_ptr)

/*
 * @infos: inits the TCP command
 *
 * @param <cmd>: the socket event
 */
#ifdef YOTTA_DEBUG
#define yotta_tcp_cmd_init(cmd) \
    {\
        ((yotta_tcp_cmd_t *) (cmd))->queue = 0; \
        yotta_tcp_cmd_set_send(cmd, 0); \
        yotta_tcp_cmd_set_release(cmd, 0); \
    }

#else
#define yotta_tcp_cmd_init(cmd)

#endif // YOTTA_DEBUG

/*
 * @infos: gets command's queue
 */
#define yotta_tcp_cmd_queue(cmd) \
    ((yotta_tcp_cmd_t *) (cmd))->queue

/*
 * @infos: sends the buffer through the TCP command's queue
 *
 * @param <cmd>: the TCP command
 * @param <buffer_size>: the buffer's size to send
 * @param <buffer_cursor>: the buffer's sending cursor
 * @param <buffer>: the buffer to send
 *
 * @returns
 *  - <0> when *buffer_cursor == buffer_size
 *  - <1> if *buffer_cursor < buffer_size
 */
uint64_t
yotta_tcp_cmd_send(yotta_tcp_cmd_t * cmd, uint64_t buffer_size, uint64_t * buffer_cursor, void const * buffer);

/*
 * @threadsafe
 * @infos: finishes the command
 *
 * @param <cmd>: the command to finish
 */
void
yotta_tcp_cmd_finish(yotta_tcp_cmd_t * cmd);

/*
 * @infos: triggers the release event
 *
 * @param <cmd>: the command to release
 */
#define yotta_tcp_cmd_release(cmd) \
    ((yotta_tcp_cmd_t *) (cmd))->release_event((yotta_tcp_cmd_t *) (cmd))


#endif
