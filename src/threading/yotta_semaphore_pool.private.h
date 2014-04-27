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
