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
#ifndef _YOTTA_SOCKET_EVENT
#define _YOTTA_SOCKET_EVENT

#include "../core/yotta_debug.h"
#include "yotta_socket.h"


/*
 * @infos: defines yotta socket event entry point function pointer type
 *
 * The socket event entry received 1 parameters:
 *  - yotta_socket_entry_t socket_event: the socket event
 */
typedef void (*yotta_socket_entry_t)(yotta_socket_event_t *);

/*
 * @infos: defines the socket's calback values
 */
struct
yotta_socket_event_s
{
    // the socket receiving events
    yotta_socket_t socket;

    // the receive event entry point
    yotta_socket_entry_t recv_event;

    // the send event entry point
    yotta_socket_entry_t send_event;

    // the exception event entry point
    yotta_socket_entry_t except_event;

    // the connection lost event entry point
    yotta_socket_entry_t lost_event;

    // the release event entry point
    yotta_socket_entry_t release_event;

    // previous and next socket
    yotta_socket_event_t * socket_next;

    // parent socket event thread
    yotta_socket_thread_t * socket_thread;
};

/*
 * @infos: initializes a socket event
 *
 * @param <socket_event>: the socket event to init
 */
#ifdef YOTTA_ASSERT
#define yotta_socket_event_init(socket_event) \
    { \
        yotta_dirty_offset( \
            socket_event, \
            sizeof(yotta_socket_t), \
            sizeof(yotta_socket_event_t) - sizeof(yotta_socket_t) \
        ); \
        ((yotta_socket_event_t *) (socket_event))->recv_event = 0; \
        ((yotta_socket_event_t *) (socket_event))->send_event = 0; \
        ((yotta_socket_event_t *) (socket_event))->except_event = 0; \
        ((yotta_socket_event_t *) (socket_event))->lost_event = 0; \
        ((yotta_socket_event_t *) (socket_event))->release_event = 0; \
        ((yotta_socket_event_t *) (socket_event))->socket_thread = 0; \
    }

#else //YOTTA_ASSERT
#define yotta_socket_event_init(socket_event) \
    yotta_dirty_offset( \
        socket_event, \
        sizeof(yotta_socket_t), \
        sizeof(yotta_socket_event_t) - sizeof(yotta_socket_t) \
    );

#endif //YOTTA_ASSERT

/*
 * @infos: sets socket event entries
 *
 * @param <socket_event>: the socket event
 * @param <function_ptr>: the function pointer
 */
#define yotta_socket_event_set_recv(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->recv_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_send(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->send_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_except(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->except_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_lost(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->lost_event = (yotta_socket_entry_t)(function_ptr)

#define yotta_socket_event_set_release(socket_event,function_ptr) \
    ((yotta_socket_event_t *) (socket_event))->release_event = (yotta_socket_entry_t)(function_ptr)

/*
 * @threadsafe
 * @infos: unlistens socket events
 *
 * @param <socket_event>: the socket event
 */
void
yotta_socket_event_unlisten(yotta_socket_event_t * socket_event);

/*
 * @infos: triggers the connection lost event
 *
 * @param <socket_event>: the socket event
 */
#define yotta_socket_event_lost(socket_event) \
    ((yotta_socket_event_t *) (socket_event))->lost_event((yotta_socket_event_t *) (socket_event))

/*
 * @infos: triggers the release event
 *
 * @param <socket_event>: the socket event
 */
#define yotta_socket_event_release(socket_event) \
    ((yotta_socket_event_t *) (socket_event))->release_event((yotta_socket_event_t *) (socket_event))

/*
 * @infos: destroyes the socket event
 *
 * @param <socket_event>: the socket event to destroy
 */
#define yotta_socket_event_destroy(socket_event) \
    yotta_socket_close(&((yotta_socket_event_t *) socket_event)->socket)

#endif
