#include <unistd.h>
#include <pthread.h>

#include "core/yotta_return.private.h"
#include "yotta_sem.private.h"
#include "yotta_sync.h"

void
yotta_sync_post(yotta_sync_t * sync)
{
    yotta_assert(sync != NULL);
    yotta_assert(sync->sem != YOTTA_SYNC_TRIGGERED);

    if(!__sync_bool_compare_and_swap(&sync->sem, YOTTA_SYNC_UNTRIGGERED, YOTTA_SYNC_TRIGGERED))
    {
        sem_post(sync->sem);
    }
}

uint64_t
yotta_sync_wait(yotta_sync_t * sync)
{
    if(sync == NULL)
    {
        yotta_return_inv_value(yotta_sync_wait, sync);
    }

    // If the semaphore has been posted,
    if(sync->sem == YOTTA_SYNC_TRIGGERED)
    {
        return YOTTA_SUCCESS;
    }

    //
    if(sync->sem != YOTTA_SYNC_UNTRIGGERED)
    {
        return yotta_return_inv_op(yotta_sync_wait);
    }

    // Else if the semaphore was not post, we fetch a semaphore from the pool
    sem_t * new_sem;
    if(yotta_sem_fetch(&new_sem) != YOTTA_SUCCESS)
    {
        yotta_return_unexpect_fail(yotta_sync_wait);
    }

    // We try to set the new semaphore to sync object,
    // if we fail that means that the semaphore has been posted while
    // fetching the new semaphore
    if(!__sync_bool_compare_and_swap(&sync->sem, YOTTA_SYNC_UNTRIGGERED, new_sem))
    {
        yotta_sem_release(new_sem);

        if(sync->sem != YOTTA_SYNC_TRIGGERED)
        {
            return yotta_return_inv_op(yotta_sync_wait);
        }

        return YOTTA_SUCCESS;
    }

    sem_wait(new_sem);

    yotta_sem_release(new_sem);

    return YOTTA_SUCCESS;
}

