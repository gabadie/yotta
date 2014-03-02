
#include <pthread.h>

#include "yotta_dispatch.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"
#include "../threading/yotta_semaphore_pool.private.h"
#include "../threading/yotta_threading.h"


/*
 * Defines a dispatch thread
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
}
yotta_dispatch_group_t;

/*
 * Defines a dispatch thread
 */
typedef struct
yotta_dispatch_thread_s
{
    // POSIX thread id
    pthread_t tid;

    // thread's local id in the group
    uint64_t local_id;

    // thread's group
    yotta_dispatch_group_t * group;

    // thread parameter
    void * user_param;
}
yotta_dispatch_thread_t;


/*
 * Locals thread variables
 */
static
__thread
yotta_dispatch_thread_t * yotta_dispatch_thread = 0;


static
void *
yotta_dispath_thread_entry(yotta_dispatch_thread_t * thread)
{
    yotta_assert(thread != 0);
    yotta_assert(thread->group != 0);

    yotta_semaphore_wait(thread->group->semaphore);

    if (thread->group->id == thread->group->group_count)
    {
        /*
         * We were not able to create all threads, then we do not launch one.
         */
        return 0;
    }

    yotta_dispatch_thread = thread;

    (thread->group->user_function)(thread->user_param);

    return 0;
}

uint64_t
yotta_dispatch(yotta_dispatch_func_t user_function, void * user_param, uint64_t user_param_stride)
{
    if (user_function == 0)
    {
        yotta_return_inv_value(yotta_dispatch, user_function);
    }
    else if (!__sync_bool_compare_and_swap(&yotta_dispatch_thread, 0, (yotta_dispatch_thread_t *) 1))
    {
        yotta_return_inv_op(yotta_dispatch);
    }

    /*
     * Creates the threads' group
     */
    yotta_dispatch_group_t group;

    /*
     * Gets the number of available cores
     */
    if (yotta_threading_cores(&group.thread_count) != YOTTA_SUCCESS)
    {
        yotta_dispatch_thread = 0;

        yotta_return_unexpect_fail(yotta_dispatch);
    }

    /*
     * Gets the group's semaphore
     */
    if (yotta_sem_fetch(&group.semaphore) != 0)
    {
        yotta_dispatch_thread = 0;

        yotta_return_unexpect_fail(yotta_dispatch);
    }

    // TODO: lets yotta_dispatch() be compatible with yotta_command()
    group.id = 0;
    group.group_count = 1;
    group.global_offset = 0;
    group.global_count = group.thread_count;
    group.user_function = user_function;

    /*
     * Creates threads
     */
    yotta_dispatch_thread_t * threads_array =
        yotta_alloc_sa(yotta_dispatch_thread_t, group.thread_count);

    uint64_t thread_created = 0;

    for ( ; thread_created < group.thread_count; thread_created++)
    {
        yotta_dispatch_thread_t * thread = threads_array + thread_created;

        thread->user_param = user_param;
        thread->local_id = thread_created;
        thread->group = &group;

        if (pthread_create(&thread->tid, 0, (void *) yotta_dispath_thread_entry, thread) != 0)
        {
            break;
        }

        user_param = ((uint8_t *) user_param) + user_param_stride;
    }

    if (thread_created != group.thread_count)
    {
        /*
         * We were not able to create all threads
         */

        group.id = group.group_count;
    }

    /*
     * Launches all threads
     */
    for (uint64_t i = 0; i < thread_created; i++)
    {
        yotta_semaphore_post(group.semaphore);
    }

    /*
     * Waits threads
     */
    for (uint64_t i = 0; i < thread_created; i++)
    {
        pthread_join(threads_array[i].tid, 0);
    }

    yotta_dispatch_thread = 0;

    yotta_sem_release(group.semaphore);

    yotta_free(threads_array);

    if (thread_created != group.thread_count)
    {
        /*
         * We were not able to create all threads
         */

        yotta_return_unexpect_fail(yotta_dispatch);
    }

    return YOTTA_SUCCESS;
}

void
yotta_get_local_id(uint64_t * out_id, uint64_t * out_count)
{
    if (yotta_dispatch_thread == 0)
    {
        if (out_id != 0)
        {
            *out_id = 0;
        }

        if (out_count != 0)
        {
            *out_count = 1;
        }

        return;
    }

    if (out_id != 0)
    {
        *out_id = yotta_dispatch_thread->local_id;
    }

    if (out_count != 0)
    {
        *out_count = yotta_dispatch_thread->group->thread_count;
    }
}

void
yotta_get_group_id(uint64_t * out_id, uint64_t * out_count)
{
    if (yotta_dispatch_thread == 0)
    {
        if (out_id != 0)
        {
            *out_id = 0;
        }

        if (out_count != 0)
        {
            *out_count = 1;
        }

        return;
    }

    if (out_id != 0)
    {
        *out_id = yotta_dispatch_thread->group->id;
    }

    if (out_count != 0)
    {
        *out_count = yotta_dispatch_thread->group->group_count;
    }
}

void
yotta_get_global_id(uint64_t * out_id, uint64_t * out_count)
{
    if (yotta_dispatch_thread == 0)
    {
        if (out_id != 0)
        {
            *out_id = 0;
        }

        if (out_count != 0)
        {
            *out_count = 1;
        }

        return;
    }

    if (out_id != 0)
    {
        *out_id = yotta_dispatch_thread->local_id +
            yotta_dispatch_thread->group->global_offset;
    }

    if (out_count != 0)
    {
        *out_count = yotta_dispatch_thread->group->global_count;
    }
}
