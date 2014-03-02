#ifndef _YOTTA_SYNC
#define _YOTTA_SYNC

#include "../yotta_prefix.h"

/*
 * @infos: Yotta synchronisation struct
 */
typedef struct
yotta_sync_s
{
    // Id of the semaphore used by the sync mechanism
    void * sem;

} yotta_sync_t;

/*
 * @infos: Wait for a yotta sync object
 *         (blocking call)
 *
 * @param <sync>: sync object to wait for
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_sync_wait(yotta_sync_t * sync);


#endif //_YOTTA_SYNC
