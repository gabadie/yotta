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
#ifndef _YOTTAPRIVATE_SYNC
#define _YOTTAPRIVATE_SYNC

#include "yotta_sync.h"
#include "../core/yotta_debug.h"


// The yotta sync object has not been triggered
#define YOTTA_SYNC_UNTRIGGERED ((void *) -3)

// The semaphore of the sync object has been triggered
#define YOTTA_SYNC_TRIGGERED ((void *) -2)


/*
 * @infos: Initialize a yotta sync object
 *
 * @param <sync>: sync object to initialize
 */
#ifdef YOTTA_DEBUG
static
inline
void
yotta_sync_init(yotta_sync_t * yotta_sync)
{
    yotta_assert((yotta_sync) != NULL);
    (yotta_sync)->sem = YOTTA_SYNC_UNTRIGGERED;
}

#else
#define yotta_sync_init(yotta_sync) \
    { \
        yotta_assert((yotta_sync) != NULL); \
        (yotta_sync)->sem = YOTTA_SYNC_UNTRIGGERED; \
    }

#endif

/*
 * @infos: Initialize and post a yotta sync object
 *
 * @param <sync>: sync object to initialize
 */
#ifdef YOTTA_DEBUG
static
inline
void
yotta_sync_init_post(yotta_sync_t * yotta_sync)
{
    yotta_assert((yotta_sync) != NULL);
    (yotta_sync)->sem = YOTTA_SYNC_TRIGGERED;
}

#else
#define yotta_sync_init_post(yotta_sync) \
    { \
        yotta_assert((yotta_sync) != NULL); \
        (yotta_sync)->sem = YOTTA_SYNC_TRIGGERED; \
    }

#endif

/*
 * @infos: Lock a yotta sync object
 *
 * @param <sync>: sync object to lock
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
void
yotta_sync_post(yotta_sync_t * sync);


#endif //_YOTTAPRIVATE_SYNC
