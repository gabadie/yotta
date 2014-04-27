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
#ifndef _YOTTA_FETCH
#define _YOTTA_FETCH

#include "../core/yotta_addr.h"
#include "../threading/yotta_sync.h"


/*
 * @infos: fetches a memory package from the master memory
 *
 * @param <master_addr>: the master address
 * @param <buffer_size>: the buffer size
 * @param <buffer_dest>: the buffer destination
 * @param <sync_finished>: the sync event oject triggered when the push is finished
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <master_addr>, <buffer_size>, <buffer_dest> or <sync> are null
 */
yotta_return_t
yotta_fetch_package(yotta_addr_t master_addr, uint64_t buffer_size, void * buffer_dest, yotta_sync_t * sync);


#endif
