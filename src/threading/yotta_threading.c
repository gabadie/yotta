
#include "yotta_threading.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"

#ifdef __APPLE__

#include <sys/sysctl.h>

uint64_t
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

uint64_t
yotta_threading_cores()
{
    yotta_todo("must implement it for linux");

    yotta_return_unexpect_fail(yotta_threading_cores);
}


#endif
