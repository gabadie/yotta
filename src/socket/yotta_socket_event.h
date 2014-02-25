#ifndef _YOTTA_SOCKET_EVENT
#define _YOTTA_SOCKET_EVENT

#include "yotta_socket.h"


/*
 * @infos; forward declaration of yotta_socket_event_t
 */
typedef struct
yotta_socket_event_s
yotta_socket_event_t;

/*
 * @infos; forward declaration of yotta_socket_thread_t
 */
typedef struct
yotta_socket_thread_s
yotta_socket_thread_t;

/*
 * @infos: defines yotta socket event entry point function pointer type
 *
 * The socket event entry received 1 parameters:
 *  - yotta_socket_entry_t socket_event: the socket event
 */
typedef void (*yotta_socket_entry_t)(yotta_socket_event_t *);

/*
 * @infos: defines the socket's calback values
 */
struct
yotta_socket_event_s
{
    // the socket receiving events
    yotta_socket_t socket;

    // the receive event entry point
    yotta_socket_entry_t recv_event;

    // the send event entry point
    yotta_socket_entry_t send_event;

    // the exception event entry point
    yotta_socket_entry_t except_event;

    // the release event entry point
    yotta_socket_entry_t release_event;

    // previous and next socket
    yotta_socket_event_t * socket_next;

    // parent socket event thread
    yotta_socket_thread_t * socket_thread;
};

/*
 * @infos: initializes a socket event
 *
 * @param <socket_event>: the socket event to init
 */
#ifdef YOTTA_DEBUG
#define yotta_socket_event_init(socket_event) \
    ((yotta_socket_event_t *) (socket_event))->socket_thread = 0;

#else
#define yotta_socket_event_init(socket_event)

#endif // YOTTA_DEBUG

/*
 * @infos: sets socket event entries
 *
 * @param <socket_event>: the socket event
 * @param <function_ptr>: the function pointer
 */
#define yotta_socket_event_set_recv(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->recv_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_send(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->send_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_except(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->except_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_release(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->release_event = (yotta_socket_entry_t)(function_ptr)

/*
 * @infos: triggers the release event
 *
 * @param <socket_event>: the socket event
 */
#define yotta_socket_event_release(socket_event) \
    ((yotta_socket_event_t *) (socket_event))->release_event((yotta_socket_event_t *) (socket_event))

/*
 * @infos: destroyes the socket event
 *
 * @param <socket_event>: the socket event to destroy
 */
#define yotta_socket_event_destroy(socket_event) \
    yotta_close_socket(&((yotta_socket_event_t *) socket_event)->socket)

#endif
