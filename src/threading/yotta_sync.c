
#include "yotta_sync.private.h"
#include "yotta_semaphore_pool.private.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_return.private.h"


void
yotta_sync_post(yotta_sync_t * sync)
{
    yotta_assert(sync != NULL);
    yotta_assert(sync->sem != YOTTA_SYNC_TRIGGERED);

    if(!__sync_bool_compare_and_swap(&sync->sem, YOTTA_SYNC_UNTRIGGERED, YOTTA_SYNC_TRIGGERED))
    {
        yotta_semaphore_post((yotta_semaphore_t *) sync->sem);

        sync->sem = YOTTA_SYNC_TRIGGERED;
    }
}

yotta_return_t
yotta_sync_wait(yotta_sync_t * sync)
{
    if(sync == NULL)
    {
        yotta_return_inv_value(yotta_sync_wait, sync);
    }

    // If the semaphore has already been triggered
    if(sync->sem == YOTTA_SYNC_TRIGGERED)
    {
        return YOTTA_SUCCESS;
    }

    // If the semaphore has been triggered between the previous call
    // and this call -> invalid operation
    if(sync->sem != YOTTA_SYNC_UNTRIGGERED)
    {
        yotta_return_inv_op(yotta_sync_wait);
    }

    // If the semaphore has not been triggered,
    // we fetch a semaphore from the pool
    yotta_semaphore_t * new_sem;

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
            yotta_return_inv_op(yotta_sync_wait);
        }

        return YOTTA_SUCCESS;
    }

    yotta_semaphore_wait((yotta_semaphore_t *) new_sem);

    sync->sem = YOTTA_SYNC_TRIGGERED;

    yotta_sem_release(new_sem);

    return YOTTA_SUCCESS;
}

