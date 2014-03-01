#ifndef _YOTTA_SEM
#define _YOTTA_SEM

#include <semaphore.h>

#include "yotta_prefix.h"

/*
 * @infos: get a POSIX semaphore from a semaphore pool
 *
 * @param <out_sem>: retrieved POSIX semaphore
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_sem_fetch(sem_t ** out_sem);

/*
 * @infos: release a POSIX semaphore into the pool
 *
 * @param <sem>: POSIX semaphore to release
 */
void
yotta_sem_release(sem_t * sem);


#endif //_YOTTA_SEM
