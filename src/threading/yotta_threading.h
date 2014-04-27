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
#ifndef _YOTTA_THREADING
#define _YOTTA_THREADING

#include "../yotta_prefix.h"


#if defined(_WIN32) || defined(_WIN64)
#error "yotta_threading.h not suported on Windows yet"

#else
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#endif

/*
 * @infos: gets the maximum number of cores
 *
 * @param <cores>[out]: the maximum number of cores on this computer
 *
 * @returns:
 *  == YOTTA_SUCCESS if succeed
 *  != YOTTA_SUCCESS if failed
 */
#define yotta_threading_cores(cores) \
    yotta_threading_max_cores(cores)

/*
 * @infos: gets the maximum number of cores
 *
 * @param <cores>[out]: the maximum number of cores on this computer
 *
 * @returns:
 *  == YOTTA_SUCCESS if succeed
 *  != YOTTA_SUCCESS if failed
 */
yotta_return_t
yotta_threading_max_cores(uint64_t * cores);


/*
 * @infos: gets the number of online cores
 *
 * @param <cores>[out]: the number of online cores on this computer
 *
 * @returns:
 *  == YOTTA_SUCCESS if succeed
 *  != YOTTA_SUCCESS if failed
 */
yotta_return_t
yotta_threading_online_cores(uint64_t * cores);

/*
 * @infos: gets the process's ID
 *
 * @returns: (uint64_t) the process's ID
 */
#define yotta_threading_pid() \
    (uint64_t) getpid()

/*
 * @infos: gets the thread's ID
 *
 * @returns: (uint64_t) the thread's ID
 */
#define yotta_threading_tid() \
    (uint64_t) pthread_self()


#endif //_YOTTA_THREADING
