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
#ifndef _YOTTA_TCP
#define _YOTTA_TCP

#include "yotta_socket.h"


/*
 * yotta_tcp_send's bitfield flags
 *  - YOTTA_TCP_NOWAIT do not block the send event
 *  - YOTTA_TCP_NOSIGNAL do not trigger SIGPIPE when connection lost
 */
#ifdef YOTTA_OSX
#define YOTTA_TCP_NOWAIT MSG_DONTWAIT
#define YOTTA_TCP_NOSIGNAL SO_NOSIGPIPE

#else //YOTTA_OSX
#define YOTTA_TCP_NOWAIT MSG_DONTWAIT
#define YOTTA_TCP_NOSIGNAL MSG_NOSIGNAL

#endif //YOTTA_OSX


/*
 * @infos: init a yotta TCP socket server
 *
 * @param <sock>: the yotta TCP socket to initialize
 * @param <port>: the port of the yotta socket
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_tcp_socket_server(sock, port) \
    yotta_socket_server_init((sock), (port), AF_UNSPEC, SOCK_STREAM)


/*
 * @infos: init a yotta TCP socket client
 *
 * @param <sock>: the yotta TCP socket to initialize
 * @param <address>: the address of yotta socket
 * @param <port>: the port of the yotta socket
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_tcp_socket_client(sock, address, port) \
    yotta_socket_client_init((sock), (address), (port), AF_UNSPEC, SOCK_STREAM)


/*
 * @infos: paired to yotta TCP socket client
 *
 * @param <sock0>: the yotta TCP socket to initialize
 * @param <sock1>: the other yotta TCP socket to initialize
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_tcp_pair(sock0, sock1) \
    yotta_socket_pair((sock0), (sock1), AF_UNSPEC, SOCK_STREAM)


/*
 * @infos: send data via a yotta TCP socket
 *
 * @param <sock>: the yotta socket used to send
 * @param <buf>: the buffer containing the data to send
 * @param <len>: the length of the data to send
 * @param <len>: the length of the data to send
 *
 * @returns:
 * the number of bytes sent
 */
#define yotta_tcp_send(sock, buf, len) \
    send((sock)->fd, (buf), (len), 0)

#define yotta_tcp_send_f(sock, buf, len, flags) \
    send((sock)->fd, (buf), (len), (flags))

/*
 * @infos: send all the data via a yotta TCP socket
 *
 * @param <sock>: the yotta socket used to send
 * @param <buf>: the buffer containing the data to send
 * @param <len>: the length of the data to send
 *
 * @returns:
 * the number of bytes sent
 *
 * @important: the socket must be in blocking mode
 */
uint64_t
yotta_tcp_sendall(yotta_socket_t * socket, void const * buffer, uint64_t buffer_size);

/*
 * @infos: receives data via a yotta TCP socket
 *
 * @param <socket>: the receiving yotta socket
 * @param <buffer>: the buffer that will contain the data to receive
 * @param <buffer_size>: the maximum buffer size
 *
 * @returns:
 *  the number of received bytes
 */
#define yotta_tcp_recv(socket, buffer, buffer_size) \
    recv((socket)->fd, (buffer), (buffer_size), 0)

/*
 * @infos: receives data via yotta TCP socket independently of the send commands
 *
 * @param <socket>: the receiving yotta socket
 * @param <buffer>: the buffer that will contain the data to receive
 * @param <buffer_size>: the maximum buffer size
 *
 * @returns:
 *  the number of received bytes
 *
 * @important: the socket must be in blocking mode
 */
uint64_t
yotta_tcp_recvall(yotta_socket_t * socket, void * buffer, uint64_t buffer_size);

/*
 * @infos: seek in comming data
 *
 * @param <socket>: the receiving yotta socket
 * @param <offset>: the seek offset
 *
 * @returns:
 *  the number of seeked bytes
 */
uint64_t
yotta_tcp_seek(yotta_socket_t * socket, uint64_t offset);

/*
 * @infos: gets the number of
 *
 * @param <socket>: the receiving yotta socket
 * @param <available_data_size>: the returned number of bytes
 *
 * @returns:
 *  the number of seeked bytes
 */
#define yotta_tcp_available(socket,available_data_size) \
    ioctl((socket)->fd, FIONREAD, available_data_size);


#endif //_YOTTA_TCP
