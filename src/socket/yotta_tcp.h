#ifndef _YOTTA_TCP
#define _YOTTA_TCP

#include "yotta_socket.h"

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
    yotta_init_socket_server((sock), (port), AF_UNSPEC, SOCK_STREAM)


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
    yotta_init_socket_client((sock), (address), (port), AF_UNSPEC, SOCK_STREAM)


/*
 * @infos: send data via a yotta TCP socket
 *
 * @param <sock>: the yotta socket used to send
 * @param <buf>: the buffer containing the data to send
 * @param <len>: the length of the data to send
 *
 * @returns:
 * the number of bytes sent
 */
#define yotta_tcp_send(sock, buf, len) \
    send((sock)->fd, (buf), (len), 0)


/*
 * @infos: recv data via a yotta TCP socket
 *
 * @param <sock>: the yotta socket used to recv
 * @param <buf>: the buffer that will contain the data to recv
 * @param <len>: the length of the data to be received
 *
 * @returns:
 * the number of bytes received
 *
 */
#define yotta_tcp_recv(sock, buf, len) \
    recv((sock)->fd, (buf), (len), 0)

int
yotta_tcp_sendall(yotta_socket_t * sock, char const * buf, int len);

int
yotta_tcp_sendall_(yotta_socket_t * sock, char const * buf, int * len);

int
yotta_tcp_recvall(yotta_socket_t * sock, void * buf, size_t len);

#endif //_YOTTA_TCP
