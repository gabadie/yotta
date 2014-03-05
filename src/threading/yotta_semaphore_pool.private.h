#ifndef _YOTTAPRIVATE_SEMAPHORE_POOL
#define _YOTTAPRIVATE_SEMAPHORE_POOL

#include "yotta_semaphore.h"


/*
 * @infos: get a yotta semaphore from a semaphore pool
 *
 * @param <out_sem>: retrieved yotta semaphore
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_sem_fetch(yotta_semaphore_t ** out_sem);

/*
 * @infos: wait on a yotta semaphore
 *
 * @param <sem>: Semaphore to wait on
 */
#define yotta_sem_wait(sem) \
{ \
    yotta_assert((sem) != NULL); \
    sem_wait((sem)); \
}

/*
 * @infos: unlock a yotta semaphore
 *
 * @param <sem>: Semaphore to unlock
 */
#define yotta_sem_post(sem) \
{ \
    yotta_assert((sem) != NULL); \
    sem_post((sem)); \
}

/*
 * @infos: release a yotta semaphore into the pool
 *
 * @param <sem>: yotta semaphore to release
 */
void
yotta_sem_release(yotta_semaphore_t * sem);

/*
 * @infos: destroy a yotta semaphore
 *
 * @param <sem>: yotta semaphore to destroy
 */
#define yotta_sem_destroy(sem) \
{ \
    yotta_assert((sem) != NULL); \
    sem_destroy((sem)); \
}

/*
 * @infos: flush the pool of semaphores
 *
 * The semaphores in use are not freed.
 *
 * @returns: the remaining number of semaphores
 */
uint64_t
yotta_sem_pool_flush();

#endif //_YOTTA_SEM
