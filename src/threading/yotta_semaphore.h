#ifndef _YOTTA_SEMAPHORE
#define _YOTTA_SEMAPHORE


#include "../yotta_prefix.h"

#if defined(__APPLE__) && defined(__MACH__)
/*
 * Mac OS X doesn't implements unnamed posix semaphores
 */
#include <mach/mach.h>
#include <mach/task.h>
#include <mach/semaphore.h>

/*
 * @infos: defines the yotta semaphore
 */
typedef semaphore_t yotta_semaphore_t;

/*
 * @infos: inits a semaphore
 *
 * @param <semaphore>: the semaphore to init
 * @param <value>: the semaphore's init value
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_init(semaphore,value) \
    semaphore_create(current_task(), (semaphore), SYNC_POLICY_FIFO, (value))

/*
 * @infos: wait a semaphore
 *
 * @param <semaphore>: the semaphore to wait on
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_wait(semaphore) \
    semaphore_wait(*(semaphore))

/*
 * @infos: posts to a semaphore
 *
 * @param <semaphore>: the semaphore to post to
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_post(semaphore) \
    semaphore_signal(*(semaphore))

/*
 * @infos: destroys a semaphore
 *
 * @param <semaphore>: the semaphore to destroy
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_destroy(semaphore) \
    semaphore_destroy(current_task(), *(semaphore))


#else

#include <semaphore.h>

/*
 * @infos: defines the yotta semaphore
 */
typedef sem_t yotta_semaphore_t;

/*
 * @infos: inits a semaphore
 *
 * @param <semaphore>: the semaphore to init
 * @param <value>: the semaphore's init value
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_init(semaphore,value) \
    sem_init((semaphore), 0, (value))

/*
 * @infos: wait a semaphore
 *
 * @param <semaphore>: the semaphore to wait on
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_wait(semaphore) \
    sem_wait(semaphore)

/*
 * @infos: posts to a semaphore
 *
 * @param <semaphore>: the semaphore to post to
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_post(semaphore) \
    sem_post(semaphore)

/*
 * @infos: destroys a semaphore
 *
 * @param <semaphore>: the semaphore to destroy
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_semaphore_destroy(semaphore) \
    sem_destroy(semaphore)


#endif

#endif
