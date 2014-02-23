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
