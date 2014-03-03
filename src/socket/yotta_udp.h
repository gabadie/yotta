#ifndef _YOTTA_UDP
#define _YOTTA_UDP

#include "../yotta_prefix.h"
#include "yotta_socket.h"

/*
 * @infos: init a yotta UDP socket server
 *
 * @param <sock>: the yotta UDP socket to initialize
 * @param <port>: the port of the yotta socket
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_udp_socket_server(sock, port) \
    yotta_socket_server_init((sock), (port), AF_UNSPEC, SOCK_DGRAM)

/*
 * @infos: init a yotta UDP socket client
 *
 * @param <sock>: the yotta UDP socket to initialize
 * @param <address>: the address of yotta socket
 * @param <port>: the port of the yotta socket
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_udp_socket_client(sock, address, port) \
    yotta_socket_client_init((sock), (address), (port), AF_UNSPEC, SOCK_DGRAM)

/*
 * @infos: send data via a yotta UDP socket
 *
 * @param <sock>: the yotta socket used to send
 * @param <buf>: the buffer containing the data to send
 * @param <len>: the length of the data to send
 *
 * @returns:
 * the number of bytes sent
 */
#define yotta_udp_send(sock, buf, len, to, to_len) \
    sendto((sock)->fd, (buf), (len), 0, (to), (to_len))


/*
 * @infos: recv data via a yotta UDP socket
 *
 * @param <sock>: the yotta socket used to recv
 * @param <buf>: the buffer that will contain the data to recv
 * @param <len>: the length of the data to be received
 *
 * @returns:
 * the number of bytes received
 *
 */
#define yotta_udp_recv(sock, buf, len, from, from_len) \
    recvfrom((sock)->fd, (buf), (len), 0, (from), (from_len))

int
yotta_udp_sendall(yotta_socket_t * sock, char const * buf, int len,
    struct sockaddr const * to, socklen_t to_len);

int
yotta_udp_sendall_(yotta_socket_t * sock, char const * buf, int * len,
    struct sockaddr const * to, socklen_t to_len);

int
yotta_udp_recvall(yotta_socket_t * sock, void * buf, size_t len,
    struct sockaddr * from, socklen_t * from_len);


#endif //_YOTTA_UDP

