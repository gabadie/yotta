#ifndef _YOTTA_SOCKET_THREAD
#define _YOTTA_SOCKET_THREAD

#include <pthread.h>

#include "yotta_socket_event.h"


/*
 * @infos: defines a socket listener thread
 */
typedef struct
yotta_socket_thread_s
{
    pthread_t id;
    pthread_mutex_t mutex;
    uint64_t quit_status;
    yotta_socket_event_t * socket_head;
    yotta_socket_event_t * current_socket;
} yotta_socket_thread_t;

/*
 * @infos: inits and launches the sockets' thread
 *
 * @param <thread>: the sockets's thread to init and launch
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
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
uint64_t
yotta_socket_thread_listen(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event);

/*
 * @threadsafe
 * @infos: unlistens socket's events
 *
 * @param <thread>: the listening sockets's thread
 * @param <socket_event>: the socket event
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_thread_unlisten(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event);

/*
 * @infos: hang until all sockets are released and destroyes the sockets' thread
 *
 * @param <thread>: the sockets's thread to destroy
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
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
uint64_t
yotta_socket_thread_kill(yotta_socket_thread_t * thread);




typedef void * (*yotta_thread_func_t)(void *);
typedef void * restrict yotta_thread_args_t;

// TODO(@tcantenot): comments
uint64_t
yotta_socket_thread_plumbing_init(yotta_socket_thread_t * thread,
    yotta_thread_func_t func, yotta_thread_args_t args);

// TODO(@tcantenot): comments
uint64_t
yotta_socket_thread_join(yotta_socket_thread_t * thread);

#endif //_YOTTA_SOCKET_THREAD
