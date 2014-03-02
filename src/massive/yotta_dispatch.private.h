#ifndef _YOTTAPRIVATE_DISPATCH
#define _YOTTAPRIVATE_DISPATCH

#include "yotta_dispatch.h"
#include "../threading/yotta_semaphore.h"
#include "../threading/yotta_thread.h"


/*
 * Defines a dispatch threads' group
 */
typedef struct
yotta_dispatch_group_s
{
    // threads' group id
    uint64_t id;

    // threads' group count
    uint64_t group_count;

    // group's threads count
    uint64_t thread_count;

    // threads group's global offset in the thread pool
    uint64_t global_offset;

    // global thread pool size
    uint64_t global_count;

    // thread function to execute
    yotta_dispatch_func_t user_function;

    // group's semaphore
    yotta_semaphore_t * semaphore;

    // number of threading waiting at a group barrier
    uint64_t waiting_threads;
}
yotta_dispatch_group_t;

/*
 * Defines a dispatch thread
 */
typedef struct
yotta_dispatch_thread_s
{
    // thread id
    yotta_thread_t tid;

    // thread's local id in the group
    uint64_t local_id;

    // thread's group
    yotta_dispatch_group_t * group;

    // thread parameter
    void * user_param;
}
yotta_dispatch_thread_t;


/*
 * Locals dispached thread variables
 */
extern
yotta_thread_local
yotta_dispatch_thread_t * yotta_dispatch_thread;


#endif
