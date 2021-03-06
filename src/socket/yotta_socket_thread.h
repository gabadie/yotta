#ifndef _YOTTA_SOCKET_THREAD
#define _YOTTA_SOCKET_THREAD

#include "yotta_socket_event.h"
#include "../threading/yotta_mutex.h"
#include "../threading/yotta_thread.h"


/*
 * @infos: defines a socket listener thread
 */
struct
yotta_socket_thread_s
{
    yotta_thread_t id;
    yotta_mutex_t mutex;
    uint64_t quit_status;
    uint64_t socket_event_count;
    yotta_socket_event_t * socket_head;
    yotta_socket_event_t * current_socket;
};

/*
 * @infos: inits and launches the sockets' thread
 *
 * @param <thread>: the sockets's thread to init and launch
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_init(yotta_socket_thread_t * thread);

/*
 * @threadsafe
 * @infos: listens socket's events
 *
 * @param <thread>: the listening sockets's thread
 * @param <socket_event>: the socket event
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_listen(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event);

/*
 * @infos: hang until all sockets are released and destroyes the sockets' thread
 *
 * @param <thread>: the sockets's thread to destroy
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_destroy(yotta_socket_thread_t * thread);

/*
 * @infos: Forces the the sockets' thread destruction
 *
 * @param <thread>: the sockets's thread to destroy
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_kill(yotta_socket_thread_t * thread);


#endif //_YOTTA_SOCKET_THREAD
