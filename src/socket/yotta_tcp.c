#include "yotta_tcp.h"

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


int
yotta_tcp_recvall(yotta_socket_t * sock, void * buf, size_t len)
{
    yotta_assert(sock != NULL);

    int size_recv;
    size_t total_size = 0;
    char chunk[YOTTA_CHUNK_SIZE];

    while(1)
    {
        memset(chunk, 0, YOTTA_CHUNK_SIZE);
        if((size_recv = recv(sock->fd, chunk, YOTTA_CHUNK_SIZE, 0)) < 0)
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
                yotta_log("yotta_tcp_recvall: received too mush data");
                memcpy(buf + total_size, chunk, len - total_size);
            }
        }

        total_size += size_recv;
    }

    return total_size;
}
