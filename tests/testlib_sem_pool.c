#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <yotta.h>
#include <mk_test.h>
#include "../src/yotta_sem.private.h"
#include "../src/core/yotta_return.h"

void
test_fetch_release()
{
    sem_t * old;
    sem_t * sem;

    yotta_sem_fetch(&old);
    sem = old;

    // Test that we always get the same semaphore
    // if we only fetch and release alternatively
    for(int i = 0u; i < 1000; i++)
    {
        yotta_sem_release(sem);
        test_assert(yotta_sem_fetch(&sem) == YOTTA_SUCCESS);
        test_assert(sem == old);
    }

    yotta_sem_release(sem);
}

void
test_many_sem()
{
    // 256 semaphores <=> 4 chunks
    uint16_t const NB_SEM = 256;

    sem_t * sem[NB_SEM];
    memset(sem, 0, sizeof(sem));

    // Ask for NB_SEM semaphores
    for(int i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem[i]) == YOTTA_SUCCESS);
        test_assert(sem[i] != NULL);
    }

    // Release all the NB_SEM semaphores into the pool
    for(int i = 0u; i < NB_SEM; i++)
    {
        yotta_sem_release(sem[i]);
    }

    sem_t * sem2[NB_SEM];
    memset(sem2, 0, sizeof(sem2));

    // Ask again for 128 semaphores and check we use the same
    // semaphores as before
    for(int i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem2[i]) == YOTTA_SUCCESS);
        test_assert(sem2[i] == sem[i]);
    }

    // Release the semaphores in reverse order
    for(int i = NB_SEM-1; i != 0; i--)
    {
        yotta_sem_release(sem2[i]);
    }

    memset(sem2, 0, sizeof(sem2));
    // Ask again for 128 semaphores and check we use the same
    // semaphores as before
    for(int i = 0u; i < NB_SEM; i++)
    {
        test_assert(yotta_sem_fetch(&sem2[i]) == YOTTA_SUCCESS);
        test_assert(sem2[i] == sem[i]);
    }

    // Release the second half of the semaphores
    for(int i = NB_SEM-1; i != NB_SEM/2; i--)
    {
        yotta_sem_release(sem2[i]);
    }

    // Ask for the same number or semaphores as previously released
    // and check that we retrieve the same semaphores as before
    for(int i = NB_SEM-1; i != NB_SEM/2; i--)
    {
        test_assert(yotta_sem_fetch(&sem2[i]) == YOTTA_SUCCESS);
        test_assert(sem2[i] == sem[NB_SEM/2 + (NB_SEM-1-i)]);
    }

    // Release all the semaphores
    for(int i = 0u; i < NB_SEM; i++)
    {
        yotta_sem_release(sem2[i]);
    }
}


int
main()
{
    test_fetch_release();
    test_many_sem();

    return 0;
}