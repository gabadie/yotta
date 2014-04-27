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

#include "yotta_threading.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../core/yotta_logger.private.h"

#ifdef YOTTA_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// See: http://lists.gnu.org/archive/html/autoconf/2002-08/msg00126.html

#ifdef _WIN32
    #ifndef _SC_NPROCESSORS_ONLN
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    #define sysconf(a) info.dwNumberOfProcessors
    #define _SC_NPROCESSORS_ONLN
    #endif
#endif

yotta_return_t
yotta_threading_max_cores(uint64_t * cores)
{
#ifdef _SC_NPROCESSORS_CONF

    int64_t nprocs = sysconf(_SC_NPROCESSORS_CONF);
    if (nprocs < 1)
    {
        yotta_logger_error("Could not determine max number of cores");
        yotta_return_unexpect_fail(yotta_threading_cores);
    }

    *cores = (uint64_t) nprocs;

    return YOTTA_SUCCESS;
#else
    yotta_logger_error("Could not determine max number of cores: Macro _SC_NPROCESSORS_CONF not defined");
    yotta_return_unexpect_fail(yotta_threading_cores);
#endif
}


yotta_return_t
yotta_threading_online_cores(uint64_t * cores)
{
#ifdef _SC_NPROCESSORS_ONLN
    int64_t nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 1)
    {
        yotta_logger_error("Could not determine number online of cores");
        yotta_return_unexpect_fail(yotta_threading_cores);
    }

    *cores = (uint64_t) nprocs;

    return YOTTA_SUCCESS;
#else
    yotta_logger_error("Could not determine number of online cores: Macro _SC_NPROCESSORS_ONLN not defined");
    yotta_return_unexpect_fail(yotta_threading_cores);
#endif
}
