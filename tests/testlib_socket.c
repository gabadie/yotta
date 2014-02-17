#include <yotta.h>
#include <mk_test.h>

#include "../src/yotta_debug.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define PORT "8001"
#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int signal)
{
    (void) signal;
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int
main(int argc, char const * const * argv)
{
    yotta_init(argc, argv);

    // Server's listening socket
    yotta_socket_t listening_socket;

    // Server socket initialization
    test_assert(yotta_tcp_socket_server(&listening_socket, PORT) == 0);

    // Server socket listen
    test_assert(yotta_listen_socket(&listening_socket, BACKLOG) != -1);

    // SIGCHLD Handler
    struct sigaction sa;
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    // Create client socket
    yotta_socket_t client_socket;
    test_assert(yotta_tcp_socket_client(&client_socket, "127.0.0.1", PORT) == 0);

    // Server's sending socket
    yotta_socket_t sending_socket;

    // Accept connection
    test_assert(yotta_accept_socket(&listening_socket, &sending_socket) == 0);

    char const * msg = "Hello, world!";

    if(fork() == 0)
    {
        // >> Child process

        // Close server's listening socket
        test_assert(yotta_close_socket(&listening_socket) == 0);

        // Send message via server's sending socket
        test_assert(yotta_tcp_send(&sending_socket, msg, strlen(msg)) != -1);

        // Close server's sending socket
        test_assert(yotta_close_socket(&sending_socket) == 0);

        exit(0);
    }

    // >> Parent process

    char buf[512] = { 0 };

    // Receive data via client socket
    test_assert(yotta_tcp_recv(&client_socket, buf, sizeof(buf)) == (int) strlen(msg));

    // Check data validity
    test_assert(memcmp(buf, msg, strlen(msg) + 1) == 0);

    // Close client socket
    test_assert(yotta_close_socket(&sending_socket) == 0);

    return 0;
}
