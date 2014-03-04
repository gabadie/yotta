#ifndef _HELPER_TCP_SOCKETS
#define _HELPER_TCP_SOCKETS

#include "testhelper_init.h"


/*
 * @infos: struct containing all testing sockets
 */
typedef struct
testhelper_tcp_sockets_s
{
    // the server listening socket
    yotta_socket_t listening_socket;

    // the server talking to client socket
    yotta_socket_t sending_socket;

    // the client side's socket
    yotta_socket_t client_socket;
} testhelper_tcp_sockets_t;


/*
 * @infos: build up testing sockets
 *
 * @param <testing_sockets>: testing socket group to build
 */
static
void
testhelper_tcp_build(testhelper_tcp_sockets_t * testing_sockets)
{
    static uint16_t listening_port = 0;
    static int32_t const BACKLOG = 16;

    test_assert(testing_sockets != 0);

    // Server socket initialization
    test_assert(yotta_tcp_socket_server(&testing_sockets->listening_socket, 0) == 0);

    // Server socket listen
    test_assert(yotta_socket_listen(&testing_sockets->listening_socket, BACKLOG) != (uint64_t) -1);

    // Get listening port
    test_assert(yotta_socket_port(&testing_sockets->listening_socket, &listening_port) == 0);
    test_assert(listening_port != 0);

    // Create client socket
    test_assert(yotta_tcp_socket_client(&testing_sockets->client_socket, "127.0.0.1", listening_port) == 0);

    // Accept connection
    test_assert(yotta_socket_accept(&testing_sockets->listening_socket, &testing_sockets->sending_socket) == 0);
}

/*
 * @infos: cleans up testing sockets
 *
 * @param <testing_sockets>: testing socket group to clean
 */
static
void
testhelper_tcp_clean(testhelper_tcp_sockets_t * testing_sockets)
{
    test_assert(testing_sockets != 0);

    test_assert(yotta_socket_close(&testing_sockets->client_socket) == 0);
    test_assert(yotta_socket_close(&testing_sockets->sending_socket) == 0);
    test_assert(yotta_socket_close(&testing_sockets->listening_socket) == 0);
}

#endif
