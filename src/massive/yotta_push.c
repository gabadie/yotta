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

#include <string.h>

#include "yotta_push.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../slave/yotta_slave_context.private.h"
#include "../whisper/yotta_whisper_push.private.h"


yotta_return_t
yotta_push_package(yotta_addr_t master_addr, uint64_t buffer_size, void const * buffer, yotta_sync_t * sync_finished)
{
    if (master_addr == 0)
    {
        yotta_return_inv_value(yotta_push_package, master_addr);
    }
    else if (buffer_size == 0)
    {
        yotta_return_inv_value(yotta_push_package, buffer_size);
    }
    else if (buffer == 0)
    {
        yotta_return_inv_value(yotta_push_package, buffer);
    }
    else if (yotta_slave_context == 0)
    {
        memcpy((void *) master_addr, buffer, buffer_size);

        if (sync_finished != 0)
        {
            yotta_sync_init_post(sync_finished);
        }

        return YOTTA_SUCCESS;
    }

    yotta_whisper_push(&yotta_slave_context->queue, master_addr, buffer_size, buffer, sync_finished);

    return YOTTA_SUCCESS;
}
