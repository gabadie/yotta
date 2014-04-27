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

#include <string.h>
#include <yotta.h>
#include <mk_test.h>

#include "testhelper_init.h"
#include "testhelper_memory.h"
#include "../src/threading/yotta_semaphore_pool.private.h"

void
test_fetch_release()
{
    yotta_semaphore_t * old;
    yotta_semaphore_t * sem;

    yotta_sem_fetch(&old);

    sem = old;

    // Test that we always get the same semaphore
    // if we only fetch and release alternatively
    for (uint64_t i = 0u; i < 1000; i++)
    {
        yotta_sem_release(sem);
        test_assert(yotta_sem_fetch(&sem) == YOTTA_SUCCESS);
        test_assert(sem == old);
    }

    yotta_sem_release(sem);

    test_assert(yotta_sem_pool_flush() == 0);
}

void
test_many_sem()
{
    // 256 semaphores <=> 4 chunks
    uint64_t const NB_SEM = 256;

    yotta_semaphore_t * sem[NB_SEM];
    memset(sem, 0, sizeof(sem));

    // Ask for NB_SEM semaphores
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem[i]) == YOTTA_SUCCESS);
        test_assert(sem[i] != NULL);
    }

    // Release all the NB_SEM semaphores into the pool
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        yotta_sem_release(sem[i]);
    }

    yotta_semaphore_t * sem2[NB_SEM];
    memset(sem2, 0, sizeof(sem2));

    // Ask again for 128 semaphores and check we use the same
    // semaphores as before
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem2[i]) == YOTTA_SUCCESS);
        test_assert(sem2[i] == sem[i]);
    }

    // Release the semaphores in reverse order
    for (int64_t i = NB_SEM-1; i >= 0; i--)
    {
        yotta_sem_release(sem2[i]);
    }

    memset(sem2, 0, sizeof(sem2));
    // Ask again for NB_SEM semaphores and check we use the same
    // semaphores as before
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem2[i]) == YOTTA_SUCCESS);
        test_assert(sem2[i] == sem[i]);
    }

    // Release the second half of the semaphores
    for (uint64_t i = NB_SEM-1; i != NB_SEM/2-1; i--)
    {
        yotta_sem_release(sem2[i]);
    }

    // Ask for the same number or semaphores as previously released
    // and check that we retrieve the same semaphores as before
    for (uint64_t i = NB_SEM-1; i != NB_SEM/2-1; i--)
    {
        test_assert(yotta_sem_fetch(&sem2[i]) == YOTTA_SUCCESS);
        test_assert(sem2[i] == sem[NB_SEM/2 + (NB_SEM-1-i)]);
    }

    // Release all the semaphores
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        yotta_sem_release(sem2[i]);
    }

    test_assert(yotta_sem_pool_flush() == 0);
}

void
test_sem_pool_flush()
{
    // 256 semaphores <=> 4 chunks
    uint64_t const NB_SEM = 256;

    yotta_semaphore_t * sem[NB_SEM];
    memset(sem, 0, sizeof(sem));

    // Ask for NB_SEM semaphores
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem[i]) == YOTTA_SUCCESS);
        test_assert(sem[i] != NULL);
    }

    // Release all the NB_SEM semaphores into the pool
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        yotta_sem_release(sem[i]);
    }

    // Flush the pool of semaphore
    test_assert(yotta_sem_pool_flush() == 0);


    // Ask for NB_SEM semaphores
    for (uint64_t i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem[i]) == YOTTA_SUCCESS);
        test_assert(sem[i] != NULL);
    }

    test_assert(yotta_sem_pool_flush() != 0);

    uint64_t nb_r = NB_SEM/10; // Number of sem to release
    test_assert2("The number of sem to release must be lower than 64 "
        "for the test to work", nb_r < 64);

    // Release nb_r semaphores into the pool
    for (uint64_t i = 0u; i < nb_r; i++)
    {
        yotta_sem_release(sem[i]);
    }

    test_assert(yotta_sem_pool_flush() != 0);

    // Ask for nb_r semaphores
    for (uint64_t i = 0u; i < nb_r; i++)
    {
        yotta_semaphore_t * old = sem[i];
        test_assert(yotta_sem_fetch(&sem[i]) == YOTTA_SUCCESS);
        test_assert(sem[i] != NULL);
        test_assert(old == sem[i]);
    }

    uint64_t rest = 42;

    // Release NB_SEM - rest semaphores into the pool
    for (uint64_t i = 0u; i < NB_SEM - rest; i++)
    {
        yotta_sem_release(sem[i]);
    }

    test_assert(yotta_sem_pool_flush() != 0);

    // Release the rest of the semaphores into the pool
    for (uint64_t i = NB_SEM - rest; i < NB_SEM; i++)
    {
        yotta_sem_release(sem[i]);
    }

    test_assert(yotta_sem_pool_flush() == 0);
}

void test_guillaume()
{
    uint64_t const NB_SEM = 20;

    yotta_semaphore_t * sem[NB_SEM];
    memset(sem, 0, sizeof(sem));

    for(uint64_t i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem[i]) == YOTTA_SUCCESS);
        test_assert(sem[i] != NULL);
    }

    yotta_semaphore_t * first = sem[0];
    yotta_semaphore_t * last  = sem[NB_SEM-1];

    yotta_sem_release(first);

    test_assert(yotta_sem_pool_flush() != 0);

    test_assert(yotta_sem_fetch(&sem[0]) == YOTTA_SUCCESS);

    test_assert(sem[0] == first);
    test_assert(sem[NB_SEM-1] == last);

    for(uint64_t i = 0u; i < NB_SEM; i++)
    {
        yotta_sem_release(sem[i]);
    }

    test_assert(yotta_sem_pool_flush() == 0);
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_fetch_release();
    testhelper_memory_check();

    test_many_sem();
    testhelper_memory_check();

    test_sem_pool_flush();
    testhelper_memory_check();

    test_guillaume();
    testhelper_memory_check();

    return 0;
}
