#ifndef _YOTTAPRIVATE_DISPATCH
#define _YOTTAPRIVATE_DISPATCH

#include "yotta_dispatch.h"
#include "../threading/yotta_semaphore.h"
#include "../threading/yotta_thread.h"


/*
 * Defines a threads global pool information attached to one thread
 */
typedef struct
yotta_global_thread_pool_s
{
    // threads' group count
    uint64_t group_id;

    // threads' group count
    uint64_t group_count;

    // threads group's global offset in the thread pool
    uint64_t global_offset;

    // global thread pool size
    uint64_t global_count;
}
yotta_global_thread_pool_t;

/*
 * Defines a dispatch threads' group
 *
 * The group is using two semaphores to implements barriers to garentee the
 * the deterministic behavior regardless the semaphores' queue policies.
 */
typedef struct
yotta_dispatch_group_s
{
    // group's global info
    yotta_global_thread_pool_t const * global;

    // group's threads count
    uint64_t thread_count;

    // thread function to execute
    yotta_dispatch_func_t user_function;

    // group's semaphores
    yotta_semaphore_t * semaphore[2];

    // waiting semaphore id
    uint64_t waiting_semaphore;

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
 * Thread's global pool
 */
extern
yotta_thread_local
yotta_global_thread_pool_t const * yotta_thread_global_pool;

/*
 * Locals dispached thread variables
 */
extern
yotta_thread_local
yotta_dispatch_thread_t const * yotta_dispatch_thread;


#endif
