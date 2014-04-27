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
#ifndef _YOTTAPRIVATE_DICTATE_QUEUE
#define _YOTTAPRIVATE_DICTATE_QUEUE

#include "../socket/yotta_tcp_queue.private.h"
#include "yotta_dictate_labels.private.h"

/*
 * @infos: defines the sizes of the receive buffer in bytes
 */
#define YOTTA_DICTATE_RECV_BUFFER_SIZE 512

/*
 *@infos: yotta_dictate_queue struct's forward declaration
 */
typedef struct
yotta_dictate_queue_s
yotta_dictate_queue_t;

/*
 * @infos: defines the yotta dictate entry pointer type
 *
 * The entry receive 1 parameter:
 *  - yotta_dictate_queue_t * <cmd_queue>: the receiving command queue
 */
typedef void (* yotta_dictate_recv_t)(
    yotta_dictate_queue_t *
);

/*
 * yotta_dictate_queue_t's receiving virtual table
 */
typedef struct
yotta_dictate_vtable_s
{
    // YOTTA_DICTATE_LABEL_DEAMON_INFO's entry point
    void (* receive_daemon_info)(yotta_dictate_queue_t * queue, uint64_t nb_computers, uint64_t nb_threads);

    // YOTTA_DICTATE_LABEL_DEAMON_ERROR's entry point
    void (* receive_daemon_error)(yotta_dictate_queue_t * queue, char const * error_msg);

    // Unknown or deprecated frame entry point
    void (* receive_unknown)(yotta_dictate_queue_t * queue);
}
yotta_dictate_vtable_t;

/*
 * @infos: defines a dictate commands queue
 *
 * Layout of a frame:
 * |  2   |     8     | xxxx|
 * |label | data_size | data|
 */
struct
yotta_dictate_queue_s
{
    // Inheritance from TCP queue
    yotta_tcp_queue_t tcp_queue;

    // Callback entry to complete the message
    yotta_dictate_recv_t callback;

    // Frame header
    struct
    {
        yotta_dictate_label_t label;
        uint64_t data_size;
    } __attribute__((packed))
    header;

    uint64_t header_cursor; // Header cursor

    // Temporary buffer
    uint8_t recv_buffer[YOTTA_DICTATE_RECV_BUFFER_SIZE];

    // Receiving virtual table
    yotta_dictate_vtable_t const * vtable;
};

/*
 * @infos: initializes a dictate command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_dictate_queue_init(yotta_dictate_queue_t * cmd_queue);

/*
 * @infos: connects a dictate command queue to a dictate master
 *
 * @param <cmd_queue>: the command queue to connect to
 * @param <ip>: the dictate master' ip
 * @param <port>: the dictate master' port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_dictate_queue_connect(yotta_dictate_queue_t * cmd_queue, char const * ip, uint16_t port);

/*
 * @infos: destroyes a dictate command queue
 *
 * @param <cmd_queue>: the command queue to destroy
 */
void
yotta_dictate_queue_destroy(yotta_dictate_queue_t * cmd_queue);

/*
 * @infos: gets the receive buffer
 *
 * @param <cmd_queue>: the command queue
 * @param <buffer_type>: the receive buffer struct to cast to
 *
 * @return:
 *  (<buffer_type> *) receive buffer
 */
#define yotta_dictate_queue_recv_buffer(cmd_queue,buffer_type) \
    (buffer_type *) (cmd_queue)->recv_buffer ; \
    yotta_assert(sizeof(buffer_type) <= YOTTA_DICTATE_RECV_BUFFER_SIZE)

/*
 * @infos: finish  in the <cmd_queue> the number of unreaded bytes
 *
 * @param <cmd_queue>: the command queue
 */
#define yotta_dictate_queue_finish(cmd_queue) \
    (cmd_queue)->callback = NULL;

#endif
