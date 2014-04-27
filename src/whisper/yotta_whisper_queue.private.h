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
#ifndef _YOTTAPRIVATE_WHISPER_QUEUE
#define _YOTTAPRIVATE_WHISPER_QUEUE

#include "../socket/yotta_tcp_queue.private.h"


/*
 * @infos: defines the sizes of the receive buffer in bytes
 */
#define YOTTA_WHISPER_RECV_BUFFER_SIZE 512

/*
 *@infos: yotta_whisper_entry_feedback_t's forward declaration
 */
typedef struct
yotta_whisper_queue_s
yotta_whisper_queue_t;

/*
 * @infos: defines the yotta wisper entry pointe type
 *
 * The logger entry received 3 parameters:
 *  - yotta_whisper_queue_t * <cmd_queue>: the receiving command queue
 */
typedef void (* yotta_whisper_recv_t)(
    yotta_whisper_queue_t *
);

/*
 * @infos: defines a whisper commands queue
 */
struct
yotta_whisper_queue_s
{
    yotta_tcp_queue_t tcp_queue;

    // callback entry to complete the message
    yotta_whisper_recv_t callback;

    // temporary buffer
    uint8_t recv_buffer[YOTTA_WHISPER_RECV_BUFFER_SIZE];
};

/*
 * @infos: initializes a whisper command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_whisper_queue_init(yotta_whisper_queue_t * cmd_queue);

/*
 * @infos: connects a whisper command queue to a whisper master
 *
 * @param <cmd_queue>: the command queue to connect to
 * @param <ip>: the whisper master' ip
 * @param <port>: the whisper master' port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_whisper_queue_connect(yotta_whisper_queue_t * cmd_queue, char const * ip, uint16_t port);

/*
 * @infos: destroyes a whisper command queue
 *
 * @param <cmd_queue>: the command queue to destroy
 */
void
yotta_whisper_queue_destroy(yotta_whisper_queue_t * cmd_queue);

/*
 * @infos: gets the receive buffer
 *
 * @param <cmd_queue>: the command queue
 * @param <buffer_type>: the receive buffer struct to cast to
 *
 * @return:
 *  (<buffer_type> *) receive buffer
 */
#define yotta_whisper_queue_recv_buffer(cmd_queue,buffer_type) \
    (buffer_type *) (cmd_queue)->recv_buffer ; \
    yotta_assert(sizeof(buffer_type) <= YOTTA_WHISPER_RECV_BUFFER_SIZE)

/*
 * @infos: finish  in the <cmd_queue> the number of unreaded bytes
 *
 * @param <cmd_queue>: the command queue
 */
#define yotta_whisper_queue_finish(cmd_queue) \
    (cmd_queue)->callback = 0;

#endif
