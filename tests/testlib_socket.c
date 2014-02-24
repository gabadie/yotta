
#include <string.h>

#include "testhelper_tcp_sockets.h"


int
main()
{
    testhelper_init();

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

    return 0;
}
