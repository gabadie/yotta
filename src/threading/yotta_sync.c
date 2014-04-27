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

