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
 * @infos: release a yotta semaphore into the pool
 *
 * @param <sem>: yotta semaphore to release
 */
void
yotta_sem_release(yotta_semaphore_t * sem);


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
