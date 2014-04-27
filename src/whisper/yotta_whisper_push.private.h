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
#ifndef _YOTTAPRIVATE_WHISPER_PUSH
#define _YOTTAPRIVATE_WHISPER_PUSH

#include "yotta_whisper_queue.private.h"
#include "../core/yotta_addr.h"
#include "../threading/yotta_sync.private.h"


/*
 * @infos: pushes <data> in the <socket>
 *
 * @param <cmd_queue>: the comamnd queue to send to the pull request to
 * @param <master_address>: absolute address in the master memory
 * @param <data_size>: the data size
 * @param <data>: the data to push
 * @param <sync>: the sync object when finished
 */
void
yotta_whisper_push(
    yotta_whisper_queue_t * cmd_queue,
    yotta_addr_t master_address,
    uint64_t data_size,
    void const * data,
    yotta_sync_t * sync
);

/*
 * @infos: processes a push
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_push_master_recv(
    yotta_whisper_queue_t * cmd_queue
);

#endif
