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
#ifndef _YOTTA_THREAD
#define _YOTTA_THREAD

#include "../yotta_prefix.h"


/*
 * Defines the yotta thread local prefix
 */
// yotta_thread_local

#if defined(__GNUC__) // GCC, clang ...
#define yotta_thread_local __thread

#elif defined(YOTTA_WINDOWS) // Visual studio
#define yotta_thread_local __declspec(thread)

#else // unknown compilor
#error "yotta_thread_local unsupported"

#endif


/*
 * @infos: defines a yotta thread struct
 */
// yotta_thread_t

/*
 * @infos: creates a yotta thread
 *
 * @param <thread_ptr>: pointer on the yotta thread
 * @param <funct_ptr>: thread function pointer
 * @param <param_ptr>: pointer parameter to pass to the thread
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
// yotta_thread_create(thread_ptr,funct_ptr,param_ptr)

/*
 * @infos: joins a yotta thread
 *
 * @param <thread>: the yotta thread to join
 */
// yotta_thread_join(thread)

#if defined(YOTTA_POSIX) // POSIX threads
#include <pthread.h>

typedef pthread_t yotta_thread_t;

#define yotta_thread_create(thread_ptr,funct_ptr,param_ptr) \
    pthread_create((thread_ptr), 0, (void *(*)(void *)) (funct_ptr), (void *) (param_ptr))

#define yotta_thread_join(thread) \
    pthread_join((thread), 0)


#elif defined(YOTTA_WINDOWS) // Windows threads
#include <windows.h>

typedef HANDLE yotta_thread_t;

#define yotta_thread_create(thread_ptr,funct_ptr,param_ptr) \
    ((*(thread_ptr) = CreateThread(0, 0, (funct_ptr), (param_ptr), 0, 0)) == 0)

#define yotta_thread_join(thread) \
    { \
        WaitForSingleObject((thread), INFINITE); \
        CloseHandle((thread)); \
    }

#else
#error "unknown yotta_thread.h configuration"

#endif


#endif
