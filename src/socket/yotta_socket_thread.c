#include "yotta_socket_thread.h"
#include "../yotta_debug.h"
#include "../yotta_return.h"

uint64_t
yotta_socket_thread_init(yotta_socket_thread_t * thread,
    yotta_thread_func_t func, yotta_thread_args_t args)
{
    yotta_assert(thread != NULL);
    yotta_assert(func != NULL);

    pthread_create(&thread->id, NULL, func, args);

    return YOTTA_SUCCESS;
}

uint64_t
yotta_socket_thread_join(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != NULL);

    pthread_join(thread->id, NULL);

    return YOTTA_SUCCESS;
}
