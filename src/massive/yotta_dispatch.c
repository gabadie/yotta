
#include <pthread.h>

#include "yotta_dispatch.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"
#include "../threading/yotta_threading.h"


/*
 * Defines a dispatch thread
 */
typedef struct
yotta_dispatch_thread_s
{
    // thread function to execute
    yotta_dispatch_func_t user_function;

    // thread parameter
    void * user_param;

    // thread's local id in the group
    uint64_t local_id;

    // thread's local group's size
    uint64_t local_count;

    // thread's group id
    uint64_t group_id;

    // thread's group's count in the pool
    uint64_t group_count;

    // thread's global id in the pool
    uint64_t global_id;

    // thread global pool size
    uint64_t global_count;

    // POSIX thread id
    pthread_t tid;
}
yotta_dispatch_thread_t;


/*
 * Checks if a dispatch is currently being executed
 */
static
uint64_t yotta_dispatched = 0;

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
    yotta_dispatch_thread = thread;

    (thread->user_function)(thread->user_param);

    return 0;
}

uint64_t
yotta_dispatch(yotta_dispatch_func_t user_function, void * user_param)
{
    if (user_function == 0)
    {
        yotta_return_inv_value(yotta_dispatch, user_function);
    }
    else if (!__sync_bool_compare_and_swap(&yotta_dispatched, 0, 1))
    {
        yotta_return_inv_op(yotta_dispatch);
    }

    uint64_t cores;

    if (yotta_threading_cores(&cores) != YOTTA_SUCCESS)
    {
        yotta_assert_return(__sync_bool_compare_and_swap(&yotta_dispatched, 1, 0), 1);

        yotta_return_unexpect_fail(yotta_dispatch);
    }

    /*
     * Creates threads
     */
    yotta_dispatch_thread_t * threads_array =
        yotta_alloc_sa(yotta_dispatch_thread_t, cores);

    for (uint64_t i = 0; i < cores; i++)
    {
        yotta_dispatch_thread_t * thread = threads_array + i;

        thread->user_function = user_function;
        thread->user_param = user_param;
        thread->local_id = i;
        thread->local_count = cores;

        // TODO: lets yotta_dispatch() be compatible with yotta_command()
        thread->group_id = 0;
        thread->group_count = 1;
        thread->global_id = i;
        thread->global_count = cores;

        pthread_create(&thread->tid, 0, (void *) yotta_dispath_thread_entry, thread);
    }

    /*
     * Waits threads
     */
    for (uint64_t i = 0; i < cores; i++)
    {
        pthread_join(threads_array[i].tid, 0);
    }

    yotta_free(threads_array);

    yotta_assert_return(__sync_bool_compare_and_swap(&yotta_dispatched, 1, 0), 1);

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
        *out_count = yotta_dispatch_thread->local_count;
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
        *out_id = yotta_dispatch_thread->group_id;
    }

    if (out_count != 0)
    {
        *out_count = yotta_dispatch_thread->group_count;
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
        *out_id = yotta_dispatch_thread->global_id;
    }

    if (out_count != 0)
    {
        *out_count = yotta_dispatch_thread->global_count;
    }
}
