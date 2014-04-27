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
#ifndef _YOTTAPRIVATE_TCP_QUEUE
#define _YOTTAPRIVATE_TCP_QUEUE

#include "yotta_socket_event.h"
#include "yotta_tcp_cmd.private.h"


/*
 * @infos: overloads yotta_socket_event_t
 */
struct
yotta_tcp_queue_s
{
    // overloads yotta_socket_event_t
    yotta_socket_event_t socket_event;

    // the first and last TCP command in the queue
    yotta_tcp_cmd_t * queue_first;
    yotta_tcp_cmd_t * queue_last;

    /*
     * the command queue's stack
     *
     * to avoid a mutex here, yotta_tcp_queue_append() do not append at the
     * queue's tail directly. Instead, it is atomically pushed to the queue's
     * stack. Then the socket thread will pop the queue's stack and append at
     * the queue's tail. Despite that stage, the first in, first out is still
     * guaranted.
     */
    yotta_tcp_cmd_t * queue_stack;
};

/*
 * @infos: inits TCP command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_tcp_queue_init(yotta_tcp_queue_t * cmd_queue);

/*
 * @threadsafe
 * @infos: appends a command in the queue
 *
 * @param <cmd_queue>: the command queue to append to
 * @param <cmd>: the command to append
 */
void
yotta_tcp_queue_append(yotta_tcp_queue_t * cmd_queue, yotta_tcp_cmd_t * cmd);

/*
 * @infos: receives the buffer through the TCP command's queue
 *
 * @param <cmd_queue>: the TCP command queue
 * @param <buffer_size>: the buffer's size to send
 * @param <buffer_cursor>: the buffer's sending cursor
 * @param <buffer>: the buffer to send
 *
 * @returns
 *  - <0> when *buffer_cursor == buffer_size
 *  - <1> if *buffer_cursor < buffer_size
 */
uint64_t
yotta_tcp_queue_recv(yotta_tcp_queue_t * cmd_queue, uint64_t buffer_size, uint64_t * buffer_cursor, void * buffer);

/*
 * @infos: async finish the TCP command queue after processing all comamnds
 *
 * @param <cmd_queue>: the command queue to close
 */
void
yotta_tcp_queue_finish(yotta_tcp_queue_t * cmd_queue);

/*
 * @infos: cleans TCP command queue
 *
 * @param <cmd_queue>: the command queue to clean
 */
void
yotta_tcp_queue_destroy(yotta_tcp_queue_t * cmd_queue);


#endif
