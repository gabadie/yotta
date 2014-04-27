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
#ifndef _HELPER_TCP_SOCKETS
#define _HELPER_TCP_SOCKETS

#include "testhelper_init.h"


/*
 * @infos: struct containing all testing sockets
 */
typedef struct
testhelper_tcp_sockets_s
{
    // the server listening socket
    yotta_socket_t listening_socket;

    // the server talking to client socket
    yotta_socket_t sending_socket;

    // the client side's socket
    yotta_socket_t client_socket;
} testhelper_tcp_sockets_t;


/*
 * @infos: build up testing sockets
 *
 * @param <testing_sockets>: testing socket group to build
 */
static
void
testhelper_tcp_build(testhelper_tcp_sockets_t * testing_sockets)
{
    static uint16_t listening_port = 0;
    static int32_t const BACKLOG = 16;

    test_assert(testing_sockets != 0);

    // Server socket initialization
    test_assert(yotta_tcp_socket_server(&testing_sockets->listening_socket, 0) == 0);

    // Server socket listen
    test_assert(yotta_socket_listen(&testing_sockets->listening_socket, BACKLOG) != (uint64_t) -1);

    // Get listening port
    test_assert(yotta_socket_host(&testing_sockets->listening_socket, NULL, &listening_port) == 0);
    test_assert(listening_port != 0);

    // Create client socket
    test_assert(yotta_tcp_socket_client(&testing_sockets->client_socket, "127.0.0.1", listening_port) == 0);

    // Accept connection
    test_assert(yotta_socket_accept(&testing_sockets->listening_socket, &testing_sockets->sending_socket) == 0);
}

/*
 * @infos: cleans up testing sockets
 *
 * @param <testing_sockets>: testing socket group to clean
 */
static
void
testhelper_tcp_clean(testhelper_tcp_sockets_t * testing_sockets)
{
    test_assert(testing_sockets != 0);

    test_assert(yotta_socket_close(&testing_sockets->client_socket) == 0);
    test_assert(yotta_socket_close(&testing_sockets->sending_socket) == 0);
    test_assert(yotta_socket_close(&testing_sockets->listening_socket) == 0);
}

#endif
