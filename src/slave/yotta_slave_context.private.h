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
#ifndef _YOTTAPRIVATE_SLAVE_CONTEXT
#define _YOTTAPRIVATE_SLAVE_CONTEXT

#include "yotta_slave.private.h"
#include "../socket/yotta_socket_thread.h"
#include "../whisper/yotta_whisper_queue.private.h"


/*
 * @infos: yotta slave context
 */
typedef struct
yotta_slave_context_s
{
    // the slave sockets' thread
    yotta_socket_thread_t thread;

    // the whisper comamnd queue
    yotta_whisper_queue_t queue;
}
yotta_slave_context_t;


/*
 * @infos: the global slave context
 *      instantiated in yotta_slave_context.c
 */
extern
yotta_slave_context_t * yotta_slave_context;


/*
 * @infos: Slave launching called if the --yotta parameters is given to yotta_init()
 *
 * @param <out_parameters>: yotta's slave parameters parsed by yotta_slave_parse_parameters()
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
yotta_return_t
yotta_slave_context_main(yotta_slave_parameters_t const * out_parameters);

#endif
