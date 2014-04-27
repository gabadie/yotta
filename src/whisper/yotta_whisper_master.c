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


#include "yotta_whisper_master.h"
#include "yotta_whisper_queue.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_memory.h"
#include "../socket/yotta_socket_thread.h"
#include "../socket/yotta_tcp.h"


static
void
yotta_whisper_master_recv(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);
    yotta_assert(master->vtable != NULL);

    yotta_logger_debug("yotta_whisper_master_recv: new slave connection");

    (master->vtable->create_queue)(master);
}

static
void
yotta_whisper_master_except(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);

    yotta_logger_error("yotta_whisper_master_except: received a TCP socket exception -> releasing");

    yotta_socket_event_release(master);
}

yotta_return_t
yotta_whisper_master_init(
    yotta_whisper_master_t * master,
    yotta_whisper_master_vtable_t const * vtable,
    uint16_t listening_port)
{
    yotta_assert(master != NULL);
    yotta_assert(vtable != NULL);
    yotta_assert(vtable->create_queue != NULL);

    yotta_dirty_s(master);

    if (yotta_tcp_socket_server(&master->socket_event.socket, listening_port) != 0)
    {
        return -1;
    }

    yotta_socket_event_init(master);
    yotta_socket_event_set_except(master, yotta_whisper_master_except);
    yotta_socket_event_set_recv(master, yotta_whisper_master_recv);
    yotta_socket_event_set_send(master, 0);

#ifdef YOTTA_ASSERT
    yotta_socket_event_set_release(master, 0);
#endif //YOTTA_ASSERT

    master->vtable = vtable;

    return 0;
}

yotta_return_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);

    return yotta_socket_event_destroy(master);
}
