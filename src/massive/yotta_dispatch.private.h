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
