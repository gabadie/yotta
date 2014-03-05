
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
