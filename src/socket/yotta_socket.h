#ifndef _YOTTA_SOCKET
#define _YOTTA_SOCKET

/*
 * @infos: Socket wrapper
 */
typedef struct
socket_s
{
    // File descriptor
    int fd;

    // Socket info
    struct addrinfo info;

} socket_t;

/*
 * @infos: TCP packet wrapper
 */
typedef struct
tcp_packet_s
{
    void * const buffer;
    size_t len;
    int flags;

} tcp_packet_t;

/*
 * @infos: UDP packet wrapper
 */
typedef struct
udp_packet_s
{
    void * const buffer;
    size_t len;
    int flags;
    struct sockaddr const * to;
    socklen_t tolen;

} udp_packet_t;


/*
 * @infos: Wrapper for TCP/UDP packets
 */
typedef struct
packet_s
{
    union
    {
        tcp_packet_t tcp;
        udp_packet_t udp;
    };

} packet_t;

/*
 * @infos: init a yotta socket
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
int yotta_init_socket(socket_s * sock, char const * address, char const * port, int family, int type);

/*
 * @infos: bind a yotta socket
 *
 * @param <sock>: the yotta socket to bind
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
int yotta_bind_socket(socket_s * sock);

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
int yotta_listen_socket(socket_s * sock, int backlog);

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
int yotta_accept_socket(socket_s * sock, socket_s * new_sock);

/*
 * @infos: TODO
 *
 * @param <sock>: the yotta context to initialize
 * @param <packet>: the yotta context to initialize
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
int yotta_send(socket_s * sock, packet_s * packet);

/*
 * @infos: close a yotta socket
 *
 * @param <sock>: the yotta socket to close
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
int yotta_close_socket(socket_s * sock);

/*
 * @infos: init a yotta TCP socket
 *
 * @param <sock>: the yotta socket to initialize
 * @param <address>: the address of yotta socket
 * @param <port>: the port of the yotta socket
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_tcp_socket(sock, address, port) \
    yotta_init_socket(sock, address, port, AF_UNSPEC, SOCK_STREAM)

/*
 * @infos: init a yotta UDP socket
 *
 * @param <sock>: the yotta socket to initialize
 * @param <address>: the address of yotta socket
 * @param <port>: the port of the yotta socket
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
#define yotta_udp_socket(sock, address, port) \
    yotta_init_socket(sock, address, port, AF_UNSPEC, SOCK_DGRAM)

#endif //_YOTTA_SOCKET
