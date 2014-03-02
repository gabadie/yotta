
#include "yotta_threading.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"

#ifdef __APPLE__

#include <sys/sysctl.h>

uint64_t
yotta_threading_cores(uint64_t * cores)
{
    int32_t mib[2];
    size_t len = sizeof(*cores);

    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;

    sysctl(mib, 2, cores, &len, 0, 0);

    if (*cores > 0)
    {
        return YOTTA_SUCCESS;
    }

    mib[1] = HW_NCPU;
    sysctl(mib, 2, cores, &len, 0, 0);

    if (*cores > 0)
    {
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
