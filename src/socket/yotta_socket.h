#ifndef _YOTTA_SOCKET
#define _YOTTA_SOCKET

#include "yotta_socket_prefix.h"

/*
 * @infos: Socket wrapper
 */
typedef struct
yotta_socket_s
{
    // File descriptor
    SOCKET fd;

    // Socket info
    struct addrinfo info;

} yotta_socket_t;

/*
 * @infos: init a yotta socket server
 *
 * @param <sock>: the yotta socket to initialize
 * @param <port>: the port of the yotta socket
 * @param <family>: the family of the yotta socket
 *                  (AF_INET (IPv4), AF_INET6 (IPv6) or AF_UNSPEC (Unspecified)
 * @param <type>: the type of the yotta socket
 *                SOCK_STREAM (TCP) or SOCK_DGRAM (UDP)
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_server_init(yotta_socket_t * sock, uint16_t port, int family, int type);

/*
 * @infos: init a yotta socket client
 *
 * @param <sock>: the yotta socket to initialize
 * @param <address>: the address of yotta socket
 * @param <port>: the port of the yotta socket
 * @param <family>: the family of the yotta socket
 *                  (AF_INET (IPv4), AF_INET6 (IPv6) or AF_UNSPEC (Unspecified)
 * @param <type>: the type of the yotta socket
 *                SOCK_STREAM (TCP) or SOCK_DGRAM (UDP)
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_client_init(yotta_socket_t * sock, char const * address,
    uint16_t port, int family, int type);

/*
 * @infos: gets the port of a yotta socket
 *
 * @param <sock>: yotta socket
 * @param <port>: output port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_port(yotta_socket_t * sock, uint16_t * port);

/*
 * @infos: listen with a yotta socket
 *
 * @param <sock>: the listening yotta socket
 * @param <backlog>: the number of pending connections the queue will hold
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_listen(yotta_socket_t * sock, int backlog);

/*
 * @infos: accept a new connection on a yotta socket
 *
 * @param <sock>: the yotta socket accepting the connection
 * @param <new_sock>: (inout) the new yotta socket. This socket will be
 *                      initialized with the connector's info.
 *                      Must not be NULL.
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_accept(yotta_socket_t * sock, yotta_socket_t * new_sock);

/*
 * @infos: close a yotta socket
 *
 * @param <sock>: the yotta socket to close
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_socket_close(yotta_socket_t * sock);

/*
 * @infos: switches socket mode to passing mode
 *
 * @param <sock>: the yotta socket to close
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_socket_nonblock(socket) \
    fcntl((socket)->fd, F_SETFL, fcntl((socket)->fd, F_GETFL, 0) | O_NONBLOCK)

/*
 * @infos: switches socket mode to blocking mode
 *
 * @param <sock>: the yotta socket to close
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_socket_block(socket) \
    fcntl((socket)->fd, F_SETFL, fcntl((socket)->fd, F_GETFL, 0) & ~O_NONBLOCK)

#endif //_YOTTA_SOCKET
