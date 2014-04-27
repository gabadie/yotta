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

#include "testhelper_tcp_sockets.h"


void
test_socket()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);
    {
        char const msg[] = "Hello, world!";
        char buf[512] = { 0 };

        test_assert(yotta_tcp_send(&sockets.sending_socket, msg, strlen(msg)) != -1);
        test_assert(yotta_tcp_recv(&sockets.client_socket, buf, sizeof(buf)) == (int) strlen(msg));

        // Check data validity
        test_assert(memcmp(buf, msg, strlen(msg) + 1) == 0);
    }
    testhelper_tcp_clean(&sockets);
}

void
test_socket_nonblocking()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);
    {
        yotta_socket_nonblock(&sockets.client_socket);

        uint64_t buf;

        test_assert(yotta_tcp_recv(&sockets.client_socket, &buf, sizeof(buf)) == -1);
    }
    testhelper_tcp_clean(&sockets);
}

void
test_socket_peer_host_listening()
{
    uint16_t const PORT = 8006;

    yotta_socket_t sock;
    yotta_tcp_socket_server(&sock, PORT);

    yotta_ipaddr_t host_ip;
    uint16_t host_port = 0;

    test_assert(yotta_socket_peer(&sock, host_ip, &host_port) != YOTTA_SUCCESS);
    test_assert(yotta_socket_host(&sock, host_ip, &host_port) == YOTTA_SUCCESS);

    //test_assert(strcmp(host_ip, "127.0.0.1") == 0 || strcmp(host_ip, "::") == 0);
    //TODO: tests host_ip bove for IPv4 or IPv6

    test_assert(host_port == PORT);

    yotta_socket_close(&sock);
}

void
test_socket_peer_host()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);
    {
        yotta_ipaddr_t host0_ip = "1";
        yotta_ipaddr_t peer0_ip = "2";
        yotta_ipaddr_t host1_ip = "3";
        yotta_ipaddr_t peer1_ip = "4";
        uint16_t host0_port = 0;
        uint16_t peer0_port = 0;
        uint16_t host1_port = 0;
        uint16_t peer1_port = 0;

        test_assert(yotta_socket_peer(&sockets.client_socket, peer0_ip, &peer0_port) == YOTTA_SUCCESS);
        test_assert(yotta_socket_host(&sockets.client_socket, host0_ip, &host0_port) == YOTTA_SUCCESS);
        test_assert(yotta_socket_peer(&sockets.sending_socket, peer1_ip, &peer1_port) == YOTTA_SUCCESS);
        test_assert(yotta_socket_host(&sockets.sending_socket, host1_ip, &host1_port) == YOTTA_SUCCESS);

        // TODO: force IPv4 on both socket to tests IPs

        test_assert(host0_port == peer1_port);
        test_assert(host1_port == peer0_port);
    }
    testhelper_tcp_clean(&sockets);
}

int
main()
{
    testhelper_init();

    test_socket();
    test_socket_nonblocking();
    test_socket_peer_host_listening();
    test_socket_peer_host();

    return 0;
}
