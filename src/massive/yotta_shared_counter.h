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
#ifndef _YOTTA_SHARED_COUNTER
#define _YOTTA_SHARED_COUNTER

#include "../core/yotta_addr.h"
#include "../threading/yotta_semaphore.h"


/*
 * Defines the yotta counter type
 */
typedef uint64_t yotta_counter_t;

/*
 * Defines the yotta counter client to have platform shared atomic counter
 */
typedef struct
yotta_shared_counter_s
{
    // the master address of the yotta_counter_t
    yotta_addr_t master_counter_addr;

    // the current cursor
    yotta_counter_t cursor;

    // the two ranges' starts
    yotta_counter_t range_start[2];

    // the ranges size
    yotta_counter_t range_size;

    union
    {
        /*
         * the ranges status bitfield
         *
         *  if range_status == 0x0 -> no ranges available and no waiting semaphore
         *  if range_status == 0x1 -> the range_start[0] is available
         *  if range_status == 0x3 -> the range_start[0] and range_start[1] is available
         */
        size_t range_status;

        /*
         * the waiting semaphore when one or more thread are waiting
         *
         *  if waiting_semaphore > 0x3
         */
        yotta_semaphore_t * waiting_semaphore;
    };

    // the minimum stock size before to trigger a new atomic range request
    yotta_counter_t stock_size;
}
yotta_shared_counter_t;

/*
 * @infos: Inits the shared counter
 *
 * @param <shared>: the shared counter to init
 * @param <master_counter_addr>: the counter's address on the master process
 * @param <range_size>: the request range size
 * @param <stock_size>: the minimum stock size before to trigger a new atomic
 *      rang request
 */
yotta_return_t
yotta_shared_counter_init(
    yotta_shared_counter_t * shared,
    yotta_addr_t master_counter_addr,
    yotta_counter_t range_size,
    yotta_counter_t stock_size
);

/*
 * @infos: Fetches and adds to the shared counter
 *
 * @param <shared>: the shared counter
 * @param <value>: the counter value
 * @param <add>: the value to add
 */
yotta_return_t
yotta_shared_counter_fetch_add(
    yotta_shared_counter_t * shared,
    yotta_counter_t * value,
    yotta_counter_t add
);


#endif //_YOTTA_SHARED_COUNTER
