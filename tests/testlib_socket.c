/*#include <yotta.h>*/
#include <mk_test.h>

#include "../src/yotta_init.h"
#include "../src/socket/yotta_socket.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "5000"
#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int signal)
{
    (void) signal;
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int
main(int argc, char const * const * argv)
{
    /*yotta_init(argc, argv);*/

    yotta_socket_t sock;
    yotta_socket_t client_sock;

    yotta_tcp_socket(&sock, "127.0.0.1", PORT);
    yotta_bind_socket(&sock);

    test_assert2("yotta_listen_socket failed", yotta_listen_socket(&sock, BACKLOG) != -1);

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

    printf("Server: waiting for connections...\n");

    yotta_accept_socket(&sock, &client_sock);

    if(fork() == 0)
    {
        // Child process

        yotta_close_socket(&sock);

        test_assert2("Send failed", send(client_sock.fd, "Hello, world!\n", 13, 0) != -1);

        yotta_close_socket(&client_sock);

        exit(0);
    }

    // Parent process

    yotta_close_socket(&client_sock);

    return 0;
}
