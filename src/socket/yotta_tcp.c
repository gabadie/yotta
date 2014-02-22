
#include "yotta_tcp.h"
#include "../yotta_debug.h"

#if 0  // TODO: unfinished work
#define YOTTA_CHUNK_SIZE 512

int
yotta_tcp_sendall(yotta_socket_t * sock, char const * buf, int len)
{
    yotta_assert(sock != NULL);

    int total = 0;       // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;

    while(total < len)
    {
        n = send(sock->fd, buf + total, bytesleft, 0);
        if(n == -1)
        {
            /*yotta_perror("yotta_tcp_socket_send");*/
            break;
        }
        total += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}


int
yotta_tcp_sendall_(yotta_socket_t * sock, char const * buf, int * len)
{
    yotta_assert(sock != NULL);

    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len)
    {
        n = send(sock->fd, buf + total, bytesleft, 0);
        if(n == -1)
        {
            /*yotta_perror("yotta_tcp_socket_send");*/
            break;
        }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}
#endif


uint64_t
yotta_tcp_recvall(yotta_socket_t * socket, void * buffer, uint64_t buffer_size)
{
    yotta_assert(socket != 0);

    uint64_t total_size = 0;

    while (buffer_size - total_size > 0)
    {
        int64_t size_recv = yotta_tcp_recv(socket, ((uint8_t *) buffer) + total_size, buffer_size - total_size);

        if (size_recv < 0)
        {
            break;
        }

        total_size += (uint64_t)size_recv;
    }

    return total_size;
}
