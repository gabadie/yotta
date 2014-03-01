#ifndef _YOTTAPRIVATE_SYNC
#define _YOTTAPRIVATE_SYNC

#include "yotta_sync.h"
#include "core/yotta_debug.h"

// The yotta sync object has not been triggered
#define YOTTA_SYNC_UNTRIGGERED 0
// The semaphore of the sync object has been triggered
#define YOTTA_SYNC_TRIGGERED -1


/*
 * @infos: Initialize a yotta sync object
 *
 * @param <sync>: sync object to initialize
 */
#define yotta_sync_init(yotta_sync) \
    { \
        yotta_assert((yotta_sync) != NULL); \
        (yotta_sync)->sem = YOTTA_SYNC_UNTRIGGERED; \
    }

/*
 * @infos: Lock a yotta sync object
 *
 * @param <sync>: sync object to lock
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
void
yotta_sync_post(yotta_sync_t * sync);

#endif //_YOTTAPRIVATE_SYNC
