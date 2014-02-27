#include "yotta_socket.h"
#include "../core/yotta_debug.h"
#include "../utils/yotta_str_utils.h"

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

uint64_t
yotta_init_socket_server(yotta_socket_t * sock, uint16_t port, int family, int type)
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
    yotta_ui16_to_str(port_str, port);

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
    memcpy(&sock->info, a, sizeof(struct addrinfo));
    sock->info.ai_next = NULL;

    freeaddrinfo(results); // Free resources

    return 0;
}

uint64_t
yotta_init_socket_client(yotta_socket_t * sock, char const * address,
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
    yotta_ui16_to_str(port_str, port);

    // Load address info structs

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
    memcpy(&sock->info, a, sizeof(struct addrinfo));
    sock->info.ai_next = NULL;

    freeaddrinfo(results); // Free resources

    return 0;
}


uint64_t
yotta_listen_socket(yotta_socket_t * sock, int backlog)
{
    yotta_assert(sock != NULL);

    if(listen(sock->fd, backlog) == -1)
    {
        yotta_perror("listen_socket");
        return -1;
    }
    return 0;
}

uint64_t
yotta_accept_socket(yotta_socket_t * sock, yotta_socket_t * new_sock)
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

    new_sock->info.ai_family = sock->info.ai_family;     // IPv4 (AF_INET), IPv6 (AF_INET6) or Unspecified (AF_UNSPEC)
    new_sock->info.ai_socktype = sock->info.ai_socktype; // TCP (SOCK_STREAM) or UDP (SOCK_DGRAM)
    new_sock->info.ai_flags = 0;
    new_sock->info.ai_protocol = 0;
    new_sock->info.ai_canonname = NULL;
    new_sock->info.ai_addr = NULL;
    new_sock->info.ai_next = NULL;
    //FIXME
    /*printf("Size of sockaddr : %d == %d\n", connector_addr_len, sizeof(connector_addr));*/
    new_sock->info.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr));
    memcpy(new_sock->info.ai_addr, (struct sockaddr *) &connector_addr, sizeof(struct sockaddr));

#if 0
    char address[INET6_ADDRSTRLEN];

    inet_ntop(connector_addr.ss_family,
        get_addr((struct sockaddr *) &connector_addr), address, sizeof(address));

    yotta_log("accept_socket: got connection from %s\n", address);
#endif

    return 0;
}


uint64_t
yotta_close_socket(yotta_socket_t * sock)
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
