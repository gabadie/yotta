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
    uint64_t quit_status;
    yotta_socket_event_t * socket_head;
} yotta_socket_thread_t;

/*
 * @infos: inits and launches the sockets' thread
 *
 * @param <thread>: the sockets's thread to init and launch
 */
uint64_t
yotta_socket_thread_init(yotta_socket_thread_t * thread);

/*
 * @infos: destroyes the sockets' thread
 *
 * @param <thread>: the sockets's thread to destroy
 */
uint64_t
yotta_socket_thread_destroy(yotta_socket_thread_t * thread);




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
