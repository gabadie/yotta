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
#ifndef _YOTTAPRIVATE_WHISPER_COMMAND
#define _YOTTAPRIVATE_WHISPER_COMMAND

#include "yotta_whisper_queue.private.h"
#include "../massive/yotta_context.h"
#include "../threading/yotta_sync.private.h"


/*
 * @infos: defines yotta whisper command entry function pointer type
 *
 *  - void * param: the user param
 */
typedef yotta_massive_command_entry_t yotta_whisper_command_entry_t;

/*
 * @infos: launches a command by a sending a command order to the other side
 *
 * @param <cmd_queue>: the comamnd queue to send to the pull request to
 * @param <function_addr>: the absolute function address
 * @param <param_size>: the parameter's size
 * @param <param>: the parameter
 * @param <sync_sent>: the sending sync object
 * @param <sync_finished>: the finish sync object
 */
void
yotta_whisper_command(
    yotta_whisper_queue_t * cmd_queue,
    yotta_whisper_command_entry_t function_addr,
    uint64_t param_size,
    void const * param,
    uint64_t group_id,
    uint64_t group_count,
    uint64_t global_offset,
    uint64_t global_count,
    yotta_sync_t * sync_sent,
    yotta_sync_t * sync_finished
);

/*
 * @infos: processes a command feedback
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_command_feedback_recv(
    yotta_whisper_queue_t * cmd_queue
);

/*
 * @infos: processes a command order
 *
 * @params: see yotta_whisper_recv_t in yotta_whisper_queue.private.h
 */
void
yotta_whisper_command_order_recv(
    yotta_whisper_queue_t * cmd_queue
);


#endif
