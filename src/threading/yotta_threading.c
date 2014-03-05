
#include "yotta_threading.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../core/yotta_logger.private.h"

#if 0
#ifdef __APPLE__

#include <sys/sysctl.h>

yotta_return_t
yotta_threading_cores(uint64_t * cores)
{
    static int32_t cores_cache = 0;

    if (cores_cache > 0)
    {
        *cores = (uint64_t) cores_cache;

        return YOTTA_SUCCESS;
    }

    int32_t cores_returned = 0;
    int32_t mib[2];
    size_t len = sizeof(cores_returned);

    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;

    sysctl(mib, 2, &cores_returned, &len, 0, 0);

    if (cores_returned > 0)
    {
        cores_cache = cores_returned;
        *cores = (uint64_t) cores_cache;

        return YOTTA_SUCCESS;
    }

    mib[1] = HW_NCPU;
    sysctl(mib, 2, &cores_returned, &len, 0, 0);

    if (cores_returned > 0)
    {
        cores_cache = cores_returned;
        *cores = (uint64_t) cores_cache;

        return YOTTA_SUCCESS;
    }

    yotta_return_unexpect_fail(yotta_threading_cores);
}


#else

yotta_return_t
yotta_threading_cores(uint64_t * cores)
{
    // TODO: must implement it for linux

    *cores = 1;

    return YOTTA_SUCCESS;
}

#endif
#endif

#ifdef _WIN32
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
#ifdef _SC_NPROCESSORS_ONLN

    int64_t nprocs = sysconf(_SC_NPROCESSORS_CONF);
    if (nprocs < 1)
    {
        yotta_logger_error("Could not determine max number of cores");
        yotta_return_unexpect_fail(yotta_threading_cores);
    }

    *cores = (uint64_t) nprocs;

    return YOTTA_SUCCESS;
#else
    yotta_logger_error("Could not determine max number of cores: Macro _SC_NPROCESSORS_ONLN not defined");
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
