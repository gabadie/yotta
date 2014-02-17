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

    yotta_socket_t listening_socket;
    yotta_socket_t sending_socket;

    yotta_tcp_socket_server(&listening_socket, PORT);

    test_assert2("yotta_listen_socket failed", yotta_listen_socket(&listening_socket, BACKLOG) != -1);

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

    yotta_log("Server: waiting for connections...\n");

    yotta_socket_t client_socket;
    yotta_tcp_socket_client(&client_socket, "127.0.0.1", PORT);

    yotta_accept_socket(&listening_socket, &sending_socket);

    if(fork() == 0)
    {
        // Child process
        yotta_close_socket(&listening_socket);

        test_assert2("Send failed", yotta_tcp_send(&sending_socket, "Hello, world!", 13) != -1);

        yotta_close_socket(&sending_socket);

        exit(0);
    }

    // Parent process

    char buf[512] = { 0 };

    yotta_tcp_recv(&client_socket, buf, sizeof(buf));

    yotta_log("Received : %s\n", buf);

    yotta_close_socket(&sending_socket);

    return 0;
}
