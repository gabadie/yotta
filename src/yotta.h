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
#ifndef _YOTTA_LIB
#define _YOTTA_LIB

#include "yotta_prefix.h"

#include "yotta_init.h"
#include "core/yotta_logger.h"
#include "core/yotta_memory.h"
#include "core/yotta_return.h"
#include "massive/yotta_context.h"
#include "massive/yotta_dispatch.h"
#include "massive/yotta_fetch.h"
#include "massive/yotta_push.h"
#include "massive/yotta_shared_counter.h"
#include "socket/yotta_socket.h"
#include "socket/yotta_tcp.h"
#include "socket/yotta_udp.h"
#include "socket/yotta_socket_thread.h"
#include "threading/yotta_atomic.h"
#include "threading/yotta_mutex.h"
#include "threading/yotta_semaphore.h"
#include "threading/yotta_sync.h"
#include "threading/yotta_thread.h"
#include "threading/yotta_threading.h"

#endif
