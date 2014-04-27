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
#ifndef _YOTTAPRIVATE_WHISPER_MASTER
#define _YOTTAPRIVATE_WHISPER_MASTER

#include "../socket/yotta_socket_event.h"


/*
 * Forward declaration of yotta_whisper_master_t
 */
typedef
struct yotta_whisper_master_s
yotta_whisper_master_t;

/*
 * @infos: defines a whisper master's virtual table
 */
typedef
struct yotta_whisper_master_vtable_s
{
    // creates the yotta_whisper_queue
    void (*create_queue)(yotta_whisper_master_t *);
}
yotta_whisper_master_vtable_t;

/*
 * @infos: defines the whisper master event
 */
struct yotta_whisper_master_s
{
    // inherits from yotta_socket_event_t
    yotta_socket_event_t socket_event;

    // yotta_whistper master's vtable
    yotta_whisper_master_vtable_t const * vtable;
};

/*
 * @infos: initializes the whisper master. the release event would have to be set
 *  with yotta_socket_event_set_release() (yotta_socket_event.h)
 *
 * @param <master>: the master to initialize
 * @param <listening_port>: the master's listening port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_whisper_master_init(
    yotta_whisper_master_t * master,
    yotta_whisper_master_vtable_t const * vtable,
    uint16_t listening_port
);

/*
 * @infos: destroyes the whisper master
 *
 * @param <master>: the master to destroyes
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master);

#endif
