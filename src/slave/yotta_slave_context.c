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

#include "yotta_slave_context.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"


/*
 * @infos: the global slave context
 */
yotta_slave_context_t * yotta_slave_context = 0;

/*
 * @infos: the global slave context without using dynamic allocation
 */
static
yotta_slave_context_t yotta_slave_context_plain;


yotta_return_t
yotta_slave_context_main(yotta_slave_parameters_t const * out_parameters)
{
    yotta_assert(out_parameters != 0);
    yotta_assert(out_parameters->client_ip != 0);
    yotta_assert(out_parameters->client_port != 0);
    yotta_assert(yotta_slave_context == 0);

    yotta_dirty_s(&yotta_slave_context_plain);

    if (yotta_socket_thread_init(&yotta_slave_context_plain.thread) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    uint64_t error = yotta_whisper_queue_connect(
        &yotta_slave_context_plain.queue,
        out_parameters->client_ip,
        out_parameters->client_port
    );

    if (error != 0)
    {
        yotta_socket_thread_destroy(&yotta_slave_context_plain.thread);
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_logger_debug("yotta_init: launching sockets' thread");

    /*
     * Sets the slave queue's release event as the destroy event because
     * <yotta_slave_context_plain> it is statically allocated
     */
    yotta_socket_event_set_release(&yotta_slave_context_plain.queue, yotta_whisper_queue_destroy);

    yotta_socket_thread_listen(&yotta_slave_context_plain.thread, (yotta_socket_event_t *) &yotta_slave_context_plain.queue);

    /*
     * Publishes the slave context to the entire application
     */
    yotta_slave_context = &yotta_slave_context_plain;

    /*
     * Run the slave sockets' thread until the queue is released
     */
    yotta_socket_thread_destroy(&yotta_slave_context_plain.thread);

    yotta_logger_debug("yotta_init: sockets' thread finished");

    return YOTTA_SUCCESS;
}
