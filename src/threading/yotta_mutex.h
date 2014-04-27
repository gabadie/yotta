/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */
#ifndef _YOTTA_MUTEX
#define _YOTTA_MUTEX


#include "../yotta_prefix.h"

#ifdef YOTTA_POSIX

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


#elif defined(YOTTA_WINDOWS)
#error "yotta_mutex.h not supported on windows yet"

#else
#error "unknown yotta_mutex.h configuration"

#endif

#endif
