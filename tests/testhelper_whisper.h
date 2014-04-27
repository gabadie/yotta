/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */
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
    static uint16_t listening_port = 0;
    static int32_t const BACKLOG = 16;

    // put some dirty content before to make sure there is no forgotten initialization
    testhelper_lorem(testing_protocol, sizeof(testhelper_whisper_protocol_t));

    yotta_socket_t listening_socket;

    test_assert(testing_protocol != 0);

    // init the sockets' thread
    yotta_socket_thread_init(&testing_protocol->thread);

    // Server socket initialization
    test_assert(yotta_tcp_socket_server(&listening_socket, 0) == 0);

    // Server socket listen
    test_assert(yotta_socket_listen(&listening_socket, BACKLOG) == 0);

    // Get listening port
    test_assert(yotta_socket_host(&listening_socket, NULL, &listening_port) == 0);
    test_assert(listening_port != 0);

    // Create queue 0's socket
    test_assert(yotta_tcp_socket_client((yotta_socket_t *) &testing_protocol->queue0, "127.0.0.1", listening_port) == 0);

    // Create queue 1's socket
    test_assert(yotta_socket_accept(&listening_socket, (yotta_socket_t *) &testing_protocol->queue1) == 0);

    // Close server socket
    test_assert(yotta_socket_close(&listening_socket) == 0);

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
    test_assert(yotta_socket_thread_destroy(&testing_protocol->thread) == 0);
}


#endif // s_HELPER_WHISPER
