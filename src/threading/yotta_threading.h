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
