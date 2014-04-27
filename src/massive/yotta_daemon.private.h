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
#ifndef _YOTTA_DAEMON
#define _YOTTA_DAEMON

#include "../yotta_prefix.h"
#include "yotta_massive_fwd.h"
#include "../dictate/yotta_dictate_queue.private.h"
#include "../threading/yotta_sync.h"
#include "../whisper/yotta_whisper_queue.private.h"

/*
 * @infos Structure containing the information about a daemon
 */
struct
yotta_daemon_s
{
    yotta_dictate_queue_t dictate_queue;
    yotta_whisper_queue_t whisper_queue;

    // the deamon's context
    yotta_context_t * context;

    // Number of available computers known by the daemon
    uint64_t available_computers;

    // Total number of available threads
    uint64_t available_threads;

    // the deamon status
    uint64_t status;

    // dictate sync
    yotta_sync_t sync_dictate_ready;

    // whisper sync
    yotta_sync_t sync_whisper_ready;
};


#define YOTTA_DAEMON_DICTATE_READY 0x1
#define YOTTA_DAEMON_WHISPER_READY 0x2


/*
 * @infos: init the given deamon
 *
 * @param <deamon>: the yotta deamon to initialize
 * @param <ip>: the yotta deamon's ip address
 * @param <port>: the yotta deamon's listening port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_daemon_init(yotta_daemon_t * daemon, yotta_context_t * context, char const * ip, uint16_t port);

/*
 * @infos: destroy the given deamon
 *
 * @param <deamon>: the yotta deamon to destroy
 */
void
yotta_daemon_destroy(yotta_daemon_t * daemon);


#endif //_YOTTA_DAEMON
