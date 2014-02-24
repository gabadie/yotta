#ifndef _YOTTA_SOCKET_THREAD
#define _YOTTA_SOCKET_THREAD

#include <pthread.h>

#include "../yotta_prefix.h"

typedef void * (*yotta_thread_func_t)(void *);
typedef void * restrict yotta_thread_args_t;

typedef struct
yotta_socket_thread_s
{
    pthread_t id;
    //yotta_socket_event_t * socket_events;

} yotta_socket_thread_t;

uint64_t
yotta_socket_thread_init(yotta_socket_thread_t * thread,
    yotta_thread_func_t func, yotta_thread_args_t args);

uint64_t
yotta_socket_thread_join(yotta_socket_thread_t * thread);

#endif //_YOTTA_SOCKET_THREAD
