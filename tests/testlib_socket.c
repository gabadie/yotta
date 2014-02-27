
#include <string.h>

#include "testhelper_tcp_sockets.h"


void
test_socket()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);
    {
        char const msg[] = "Hello, world!";
        char buf[512] = { 0 };

        test_assert(yotta_tcp_send(&sockets.sending_socket, msg, strlen(msg)) != -1);
        test_assert(yotta_tcp_recv(&sockets.client_socket, buf, sizeof(buf)) == (int) strlen(msg));

        // Check data validity
        test_assert(memcmp(buf, msg, strlen(msg) + 1) == 0);
    }
    testhelper_tcp_clean(&sockets);
}

void
test_socket_nonblocking()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);
    {
        yotta_socket_nonblock(&sockets.client_socket);

        uint64_t buf;

        test_assert(yotta_tcp_recv(&sockets.client_socket, &buf, sizeof(buf)) == -1);
    }
    testhelper_tcp_clean(&sockets);
}

int
main()
{
    testhelper_init();

    test_socket();
    test_socket_nonblocking();

    return 0;
}
