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

#include "yotta_shared_counter.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../slave/yotta_slave_context.private.h"
#include "../threading/yotta_atomic.h"


yotta_return_t
yotta_shared_counter_init(
    yotta_shared_counter_t * shared,
    yotta_addr_t master_counter_addr,
    yotta_counter_t range_size,
    yotta_counter_t stock_size
)
{
    if (shared == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_init, shared)
    }
    else if (master_counter_addr == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_init, master_counter_addr)
    }
    else if (range_size == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_init, range_size)
    }
    else if (stock_size > range_size)
    {
        yotta_return_inv_value(yotta_shared_counter_init, stock_size)
    }

    yotta_dirty_s(shared);

    shared->master_counter_addr = master_counter_addr;

    if (yotta_slave_context == 0)
    {
        /*
         * On the master process, only master_counter_addr maters
         */
        return YOTTA_SUCCESS;
    }

    shared->range_size = range_size;
    shared->range_status = 0;
    shared->stock_size = stock_size;

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_shared_counter_fetch_add(yotta_shared_counter_t * shared, yotta_counter_t * value, yotta_counter_t add)
{
    if (shared == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_fetch_add, shared)
    }
    else if (value == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_fetch_add, value)
    }
    else if (add == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_fetch_add, add)
    }
    else if (yotta_slave_context == 0)
    {
        /*
         * On the master process, we directly use the xadd instruction
         */
        yotta_counter_t * atomic = (yotta_counter_t *) shared->master_counter_addr;

        *value = yotta_atomic_fetch_add(atomic, add);

        return YOTTA_SUCCESS;
    }

    yotta_todo("implement yotta_whisper_counter_fetch_add()");

    return YOTTA_SUCCESS;
}
