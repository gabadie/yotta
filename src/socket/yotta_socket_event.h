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

    // previous and next socket
    yotta_socket_event_t * socket_next;
    yotta_socket_event_t * socket_previous;
};

/*
 * @infos: sets socket event entries
 *
 * @param <socket_event>: the socket event
 * @param <function_ptr>: the function pointer
 */
#define yotta_socket_event_recv(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->recv_event = (yotta_socket_entry_t)(function_ptr);

#define yotta_socket_event_send(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->send_event = (yotta_socket_entry_t)(function_ptr);

#define yotta_socket_event_except(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->except_event = (yotta_socket_entry_t)(function_ptr);

/*
 * @infos: destroyes the socket event
 *
 * @param <socket_event>: the socket event to destroy
 */
#define yotta_socket_event_destroy(socket_event) \
    yotta_close_socket(&((yotta_socket_event_t *) socket_event)->socket)

#endif
