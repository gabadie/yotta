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
#ifndef _YOTTA_SOCKET_THREAD
#define _YOTTA_SOCKET_THREAD

#include "yotta_socket_event.h"
#include "../threading/yotta_mutex.h"
#include "../threading/yotta_thread.h"


/*
 * @infos: defines a socket listener thread
 */
struct
yotta_socket_thread_s
{
    yotta_thread_t id;
    yotta_mutex_t mutex;
    uint64_t quit_status;
    uint64_t socket_event_count;
    yotta_socket_event_t * socket_head;
    yotta_socket_event_t * current_socket;
};

/*
 * @infos: inits and launches the sockets' thread
 *
 * @param <thread>: the sockets's thread to init and launch
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_init(yotta_socket_thread_t * thread);

/*
 * @threadsafe
 * @infos: listens socket's events
 *
 * @param <thread>: the listening sockets's thread
 * @param <socket_event>: the socket event
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_listen(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event);

/*
 * @infos: hang until all sockets are released and destroyes the sockets' thread
 *
 * @param <thread>: the sockets's thread to destroy
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_destroy(yotta_socket_thread_t * thread);

/*
 * @infos: Forces the the sockets' thread destruction
 *
 * @param <thread>: the sockets's thread to destroy
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_socket_thread_kill(yotta_socket_thread_t * thread);


#endif //_YOTTA_SOCKET_THREAD
