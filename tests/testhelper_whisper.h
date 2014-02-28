#ifndef _HELPER_WHISPER
#define _HELPER_WHISPER

#include "testhelper_init.h"
#include "testhelper_lorem.h"
#include "../src/socket/yotta_socket_thread.h"
#include "../src/whisper/yotta_whisper_queue.private.h"


/*
 * @infos: struct containing all testing sockets
 */
typedef struct
testhelper_whisper_protocol_s
{
    // sockets' thread
    yotta_socket_thread_t thread;

    // whisper queue
    yotta_whisper_queue_t queue0;
    yotta_whisper_queue_t queue1;
}
testhelper_whisper_protocol_t;


/*
 * @infos: builds up a whisper protocol between 2 queues
 *
 * @param <testing_protocol>: the testing protocol
 */
static
void
testhelper_whisper_protocol_init(testhelper_whisper_protocol_t * testing_protocol)
{
    static uint16_t PORT = 8001;
    static int32_t const BACKLOG = 16;

    // put some dirty content before to make sure there is no forgotten initialization
    testhelper_lorem(testing_protocol, sizeof(testhelper_whisper_protocol_t));

    yotta_socket_t listening_socket;

    test_assert(testing_protocol != 0);

    // init the sockets' thread
    yotta_socket_thread_init(&testing_protocol->thread);

    // Server socket initialization
    test_assert(yotta_tcp_socket_server(&listening_socket, PORT) == 0);

    // Server socket listen
    test_assert(yotta_listen_socket(&listening_socket, BACKLOG) == 0);

    // Create queue 0's socket
    test_assert(yotta_tcp_socket_client((yotta_socket_t *) &testing_protocol->queue0, "127.0.0.1", PORT) == 0);

    // Create queue 1's socket
    test_assert(yotta_accept_socket(&listening_socket, (yotta_socket_t *) &testing_protocol->queue1) == 0);

    // Close server socket
    test_assert(yotta_close_socket(&listening_socket) == 0);

    // inits queues
    yotta_whisper_queue_init(&testing_protocol->queue0);
    yotta_whisper_queue_init(&testing_protocol->queue1);

    yotta_socket_event_set_release(&testing_protocol->queue0, yotta_whisper_queue_destroy);
    yotta_socket_event_set_release(&testing_protocol->queue1, yotta_whisper_queue_destroy);

    // listen both queues
    yotta_socket_thread_listen(&testing_protocol->thread, (yotta_socket_event_t *) &testing_protocol->queue0);
    yotta_socket_thread_listen(&testing_protocol->thread, (yotta_socket_event_t *) &testing_protocol->queue1);
}

/*
 * @infos: cleans up a whisper protocol
 *
 * @param <testing_protocol>: the testing protocol
 */
static
void
testhelper_whisper_protocol_destroy(testhelper_whisper_protocol_t * testing_protocol)
{
    /*
     * Closes queues
     *
     * we close only one because the other will be then closed by an exception
     */
    yotta_tcp_queue_finish((yotta_tcp_queue_t *) &testing_protocol->queue0);

    // cleans the sockets' thread
    yotta_socket_thread_destroy(&testing_protocol->thread);
}


#endif // s_HELPER_WHISPER
