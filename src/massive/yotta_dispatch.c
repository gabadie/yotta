/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */

#include "yotta_dispatch.private.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.private.h"
#include "../threading/yotta_semaphore_pool.private.h"
#include "../threading/yotta_threading.h"


yotta_thread_local
yotta_global_thread_pool_t const * yotta_thread_global_pool = 0;

yotta_thread_local
yotta_dispatch_thread_t const * yotta_dispatch_thread = 0;


static
void *
yotta_dispath_thread_entry(yotta_dispatch_thread_t * thread)
{
    yotta_assert(thread != 0);
    yotta_assert(thread->group != 0);

    yotta_semaphore_wait(thread->group->semaphore[0]);

    if (thread->group->thread_count == 0)
    {
        /*
         * We were not able to create all threads, then we do not launch one.
         */
        return 0;
    }

    yotta_dispatch_thread = thread;
    yotta_thread_global_pool = thread->group->global;

    (thread->group->user_function)(thread->user_param);

    return 0;
}

yotta_return_t
yotta_dispatch(yotta_dispatch_func_t user_function, void * user_param, uint64_t user_param_stride)
{
    if (user_function == 0)
    {
        yotta_return_inv_value(yotta_dispatch, user_function);
    }
    else if (yotta_dispatch_thread != 0)
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
        yotta_return_unexpect_fail(yotta_dispatch);
    }

    /*
     * Gets the first group's semaphore
     */
    if (yotta_sem_fetch(&group.semaphore[0]) != 0)
    {
        yotta_return_unexpect_fail(yotta_dispatch);
    }

    /*
     * Gets the second group's semaphore
     */
    if (yotta_sem_fetch(&group.semaphore[1]) != 0)
    {
        yotta_sem_release(group.semaphore[0]);
        yotta_return_unexpect_fail(yotta_dispatch);
    }

    group.global = yotta_thread_global_pool;
    group.user_function = user_function;
    group.waiting_semaphore = 1;
    group.waiting_threads = 0;

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

        if (yotta_thread_create(&thread->tid, yotta_dispath_thread_entry, thread) != 0)
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

        group.thread_count = 0;
    }

    /*
     * Launches all threads
     */
    for (uint64_t i = 0; i < thread_created; i++)
    {
        yotta_semaphore_post(group.semaphore[0]);
    }

    /*
     * Waits threads
     */
    for (uint64_t i = 0; i < thread_created; i++)
    {
        yotta_thread_join(threads_array[i].tid);
    }

    yotta_sem_release(group.semaphore[0]);
    yotta_sem_release(group.semaphore[1]);


    yotta_free(threads_array);

    if (group.thread_count == 0)
    {
        /*
         * We were not able to create all threads
         */

        yotta_return_unexpect_fail(yotta_dispatch);
    }

    return YOTTA_SUCCESS;
}

void
yotta_group_barrier()
{
    if (yotta_dispatch_thread == 0)
    {
        /*
         * The barrier is not blocking if not a dispatched thread
         */
        return;
    }

    yotta_assert(yotta_dispatch_thread->group != 0);

    yotta_dispatch_group_t * group = yotta_dispatch_thread->group;

    uint64_t waiting = __sync_add_and_fetch(&group->waiting_threads, 1);

    if (waiting != group->thread_count)
    {
        /*
         * This thread is not the last to arrive, then it is waiting
         */
        yotta_semaphore_wait(group->semaphore[group->waiting_semaphore]);
        return;
    }

    /*
     * This thread is the last to arrive, then it is releasing its friends
     */
    group->waiting_semaphore = !group->waiting_semaphore;
    group->waiting_threads = 0;

    for (uint64_t i = 1; i < waiting; i++)
    {
        yotta_semaphore_post(group->semaphore[!group->waiting_semaphore]);
    }
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

    yotta_assert(yotta_dispatch_thread->group != 0);

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
    if (yotta_thread_global_pool == 0)
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
        *out_id = yotta_thread_global_pool->group_id;
    }

    if (out_count != 0)
    {
        *out_count = yotta_thread_global_pool->group_count;
    }
}

void
yotta_get_global_id(uint64_t * out_id, uint64_t * out_count)
{
    if (out_id != 0)
    {
        if (yotta_dispatch_thread != 0)
        {
            *out_id = yotta_dispatch_thread->local_id;
        }
        else
        {
            *out_id = 0;
        }

        if (yotta_thread_global_pool != 0)
        {
            *out_id += yotta_thread_global_pool->global_offset;
        }
    }

    if (out_count != 0)
    {
        if (yotta_thread_global_pool != 0)
        {
            *out_count = yotta_thread_global_pool->global_count;
        }
        else if (yotta_dispatch_thread != 0)
        {
            *out_count = yotta_dispatch_thread->group->thread_count;
        }
        else
        {
            *out_count = 1;
        }
    }
}
