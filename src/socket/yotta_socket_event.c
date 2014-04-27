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

#include "yotta_socket_event.h"
#include "yotta_socket_thread.h"
#include "../core/yotta_debug.h"


void
yotta_socket_event_unlisten(yotta_socket_event_t * socket_event)
{
    yotta_assert(socket_event != 0);
    yotta_assert(socket_event->socket_thread != 0);

    yotta_socket_thread_t * thread = socket_event->socket_thread;

    yotta_mutex_lock(&thread->mutex);
    {
        yotta_assert(thread->socket_event_count > 0);

        yotta_socket_event_t ** parent_ptr = &thread->socket_head;

        while (*parent_ptr != socket_event)
        {
            /*
             * Check if unable to find the socket event in its own socket thread.
             */
            yotta_assert(*parent_ptr != 0);

            parent_ptr = &(*parent_ptr)->socket_next;
        }

        *parent_ptr = socket_event->socket_next;

        /*
         * the socket thread might be currently working excatly on this
         * socket event. In this case, we have to change it's iterator
         */
        if (thread->current_socket == socket_event)
        {
            thread->current_socket = socket_event->socket_next;
        }

        thread->socket_event_count--;
    }
    yotta_mutex_unlock(&thread->mutex);

#ifdef YOTTA_ASSERT
    socket_event->socket_thread = 0;
#endif //YOTTA_ASSERT

    yotta_assert(socket_event->socket_thread == 0);
}
