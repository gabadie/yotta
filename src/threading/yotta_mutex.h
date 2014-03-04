#ifndef _YOTTA_MUTEX
#define _YOTTA_MUTEX


#include "../yotta_prefix.h"

#if defined(_WIN32) || defined(_WIN64)
#error "yotta_mutex.h not supported on windows yet"

#else

#include <pthread.h>

/*
 * @infos: defines the yotta mutex
 */
typedef pthread_mutex_t yotta_mutex_t;

/*
 * @infos: Inits a yotta mutex
 */
#define YOTTA_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

/*
 * @infos: inits a mutex
 *
 * @param <mutex>: the mutex ptr to init
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_mutex_init(mutex) \
    pthread_mutex_init((mutex), 0)

/*
 * @infos: locks a mutex
 *
 * @param <mutex>: the mutex ptr to lock
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_mutex_lock(mutex) \
    pthread_mutex_lock(mutex)

/*
 * @infos: tries to lock a mutex
 *
 * @param <mutex>: the mutex ptr to try to lock
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_mutex_trylock(mutex) \
    pthread_mutex_trylock(mutex)

/*
 * @infos: unlocks a mutex
 *
 * @param <mutex>: the mutex ptr to unlock
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_mutex_unlock(mutex) \
    pthread_mutex_unlock(mutex)

/*
 * @infos: destroys a semaphore
 *
 * @param <semaphore>: the mutex ptr to destroy
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
#define yotta_mutex_destroy(mutex) \
    pthread_mutex_destroy(mutex)


#endif

#endif
