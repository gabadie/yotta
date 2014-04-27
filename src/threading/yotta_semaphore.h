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
#ifndef _YOTTA_SEMAPHORE
#define _YOTTA_SEMAPHORE


#include "../yotta_prefix.h"

#if defined(YOTTA_OSX)
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


#elif defined(YOTTA_POSIX)

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


#elif defined(YOTTA_WINDOWS)
#error "yotta_semaphore.h not supported on windows yet"

#else
#error "unknown yotta_semaphore.h configuration"

#endif

#endif
