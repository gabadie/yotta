#include "yotta_socket.h"
#include "../core/yotta_debug.h"
#include "../utils/yotta_str_utils.h"
#include "../core/yotta_return.private.h"

#include <string.h>
#include <stdlib.h>

#ifdef YOTTA_DEBUG
// Get sockaddr, IPv4 or IPv6:
void *
get_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
#endif

yotta_return_t
yotta_socket_server_init(yotta_socket_t * sock, uint16_t port, int family, int type)
{
    yotta_assert(sock != NULL);

    int sockfd;
    struct addrinfo hints, *a;
    struct addrinfo * results; // Linked list of addrinfo
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = family;                  // IPv4 (AF_INET), IPv6 (AF_INET6) or Unspecified (AF_UNSPEC)
    hints.ai_socktype = type;                  // TCP (SOCK_STREAM) or UDP (SOCK_DGRAM)
    hints.ai_flags = AI_PASSIVE;               // Set IP automatically
    hints.ai_protocol = 0;                     // Any protocol
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    char port_str[6];
    yotta_ui64_to_str(port_str, (uint64_t) port, 10);

    // Load address info structs
    if((rv = getaddrinfo(NULL, port_str, &hints, &results)) != 0)
    {
        yotta_log("getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // Loop through all the results and use the first valid one
    for(a = results; a != NULL; a = a->ai_next)
    {
        if((sockfd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1)
        {
            yotta_perror("server: socket");
            continue;
        }

        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            yotta_perror("setsockopt");
            continue;
        }

        if(type == SOCK_STREAM)
        {
            if(bind(sockfd, a->ai_addr, a->ai_addrlen) == -1)
            {
                close(sockfd);
                yotta_perror("server: bind");
                continue;
            }
        }

        break;
    }

    if(a == NULL)
    {
        freeaddrinfo(results); // Free resources
        yotta_log("Failed to create yotta socket server");
        return -1;
    }

    // Set the out socket
    sock->fd = sockfd;

    freeaddrinfo(results); // Free resources

    return 0;
}

yotta_return_t
yotta_socket_client_init(yotta_socket_t * sock, char const * address,
    uint16_t port, int family, int type)
{
    int sockfd;
    struct addrinfo hints, *a;
    struct addrinfo * results; // Linked list of addrinfo
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = type;

    char port_str[6];
    yotta_ui64_to_str(port_str, (uint64_t) port, 10);

    // Load address info structs
    if((rv = getaddrinfo(address, port_str, &hints, &results)) != 0)
    {
        yotta_log("getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // Loop through all the results and use the first valid one
    for(a = results; a != NULL; a = a->ai_next)
    {
        if((sockfd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1)
        {
            yotta_perror("client: socket");
            continue;
        }

        if(connect(sockfd, a->ai_addr, a->ai_addrlen) == -1)
        {
            closesocket(sockfd);
            yotta_perror("client: connect");
            continue;
        }

        break;
    }

    if(a == NULL)
    {
        freeaddrinfo(results); // Free resources
        yotta_log("Failed to create yotta socket client");
        return -1;
    }

#if 0
    char addr[INET6_ADDRSTRLEN];

    inet_ntop(a->ai_family,
        get_addr((struct sockaddr *)a->ai_addr), addr, sizeof(addr));

    yotta_log("client: connecting to %s\n", addr);
#endif

    // Set the out socket
    sock->fd = sockfd;

    freeaddrinfo(results); // Free resources

    return 0;
}

yotta_return_t
yotta_socket_pair(yotta_socket_t * sock0, yotta_socket_t * sock1, int family, int type)
{
    yotta_assert(sock0 != NULL);
    yotta_assert(sock1 != NULL);

    int fds[2];

    if (socketpair(family, type, 0, fds))
    {
        return -1;
    }

    sock0->fd = fds[0];
    sock1->fd = fds[1];

    return 0;
}

static
void
yotta_socket_parse(struct sockaddr_storage const * sin, yotta_ipaddr_t ip_address, uint16_t * port)
{
    yotta_assert(sin != NULL);
    yotta_assert(ip_address != NULL || port != NULL);

    if (sin->ss_family == AF_INET)
    {
        struct sockaddr_in *s = (struct sockaddr_in *) sin;

        if (port != NULL)
        {
            *port = ntohs(s->sin_port);
        }

        if (ip_address != NULL)
        {
            inet_ntop(AF_INET, &s->sin_addr, ip_address, YOTTA_IPADDR_STRLEN);
        }
    }
    else
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *) sin;

        if (port != NULL)
        {
            *port = ntohs(s->sin6_port);
        }

        if (ip_address != NULL)
        {
            inet_ntop(AF_INET6, &s->sin6_addr, ip_address, YOTTA_IPADDR_STRLEN);
        }
    }
}

yotta_return_t
yotta_socket_peer(yotta_socket_t * sock, yotta_ipaddr_t ip_address, uint16_t * port)
{
    yotta_assert(sock != NULL);
    yotta_assert(ip_address != NULL || port != NULL);

    struct sockaddr_storage sin;
    socklen_t sin_len = sizeof(sin);

    if (getpeername(sock->fd, (struct sockaddr *) &sin, &sin_len) == -1)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_socket_parse(&sin, ip_address, port);

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_socket_host(yotta_socket_t * sock, yotta_ipaddr_t ip_address, uint16_t * port)
{
    yotta_assert(sock != NULL);
    yotta_assert(ip_address != NULL || port != NULL);

    struct sockaddr_storage sin;
    socklen_t sin_len = sizeof(sin);

    if (getsockname(sock->fd, (struct sockaddr *) &sin, &sin_len) == -1)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_socket_parse(&sin, ip_address, port);

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_socket_listen(yotta_socket_t * sock, int backlog)
{
    yotta_assert(sock != NULL);

    if(listen(sock->fd, backlog) == -1)
    {
        yotta_perror("listen_socket");
        return -1;
    }
    return 0;
}

yotta_return_t
yotta_socket_accept(yotta_socket_t * sock, yotta_socket_t * new_sock)
{
    yotta_assert(sock != NULL && new_sock != NULL);

    int new_fd;
    struct sockaddr_storage connector_addr;
    socklen_t connector_addr_len;

    connector_addr_len = sizeof(connector_addr);

    new_fd = accept(sock->fd, (struct sockaddr *) &connector_addr, &connector_addr_len);

    if (new_fd == -1)
    {
        yotta_perror("accept_socket");
        return -1;
    }

    new_sock->fd = new_fd;

#if 0
    char address[INET6_ADDRSTRLEN];

    inet_ntop(connector_addr.ss_family,
        get_addr((struct sockaddr *) &connector_addr), address, sizeof(address));

    yotta_log("accept_socket: got connection from %s\n", address);
#endif

    return 0;
}


yotta_return_t
yotta_socket_close(yotta_socket_t * sock)
{
    yotta_assert(sock != NULL);

    closesocket(sock->fd);

    /*if(sock->info.ai_addr != NULL)
    {
        freeaddrinfo(sock->info);
        free(sock->info.ai_addr);
    }*/

    return 0;
}
