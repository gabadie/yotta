#include <string.h>

#include "yotta_udp.h"
#include "../yotta_debug.h"

#define YOTTA_CHUNK_SIZE 512

int
yotta_udp_sendall(yotta_socket_t * sock, char const * buf, int len,
    struct sockaddr const * to, socklen_t to_len)
{
    yotta_assert(sock != NULL);

    int total = 0;       // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;

    while(total < len)
    {
        n = sendto(sock->fd, buf + total, bytesleft, 0, to, to_len);
        if(n == -1)
        {
            /*yotta_perror("yotta_udp_socket_send");*/
            break;
        }
        total += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}


int
yotta_udp_sendall_(yotta_socket_t * sock, char const * buf, int * len,
    struct sockaddr const * to, socklen_t to_len)
{
    yotta_assert(sock != NULL);

    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len)
    {
        n = sendto(sock->fd, buf + total, bytesleft, 0, to, to_len);
        if(n == -1)
        {
            /*yotta_perror("yotta_udp_socket_send");*/
            break;
        }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}


int
yotta_udp_recvall(yotta_socket_t * sock, void * buf, size_t len,
    struct sockaddr * from, socklen_t * from_len)
{
    yotta_assert(sock != NULL);

    int size_recv;
    size_t total_size = 0;
    char chunk[YOTTA_CHUNK_SIZE];

    while(1)
    {
        memset(chunk, 0, YOTTA_CHUNK_SIZE);
        if((size_recv = recvfrom(sock->fd, chunk, YOTTA_CHUNK_SIZE, 0, from, from_len)) < 0)
        {
            break;
        }
        else
        {
            if(total_size + size_recv <= len)
            {
                memcpy(buf + total_size, chunk, size_recv);
            }
            else
            {
                yotta_log("yotta_udp_recvall: received too much data");
                memcpy(buf + total_size, chunk, len - total_size);
            }
        }

        total_size += size_recv;
    }

    return total_size;
}

