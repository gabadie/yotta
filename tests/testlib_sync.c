#include <yotta.h>
#include <mk_test.h>
#include "../src/yotta_sync.private.h"

#include <pthread.h>
#include <semaphore.h>


sem_t synchro;

void * producer_func(void * s)
{
    yotta_sync_t * yotta_sync = (yotta_sync_t *) s;

    yotta_sync_init(yotta_sync);

    yotta_assert(yotta_sync->sem == (sem_t *) YOTTA_SYNC_UNTRIGGERED);

    sem_post(&synchro);

    yotta_assert(yotta_sync_wait(yotta_sync) == YOTTA_SUCCESS);

    yotta_assert(yotta_sync->sem == (sem_t *) YOTTA_SYNC_TRIGGERED);

    return NULL;
}

void * consumer_func(void * s)
{
    sem_wait(&synchro);

    yotta_sync_t * yotta_sync = (yotta_sync_t *) s;

    yotta_sync_post(yotta_sync);

    return NULL;
}

void test_post_wait()
{
    yotta_sync_t yotta_sync;

    #pragma GCC diagnostic ignored "-Waddress"
    // This call check if the yotta_sync_t is not null
    // GCC output: warning: the comparison will always evaluate as ‘true’ for the address of ‘yotta_sync’ will never be NULL [-Waddress]
    yotta_sync_init(&yotta_sync);

    yotta_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_UNTRIGGERED);

    yotta_sync_post(&yotta_sync);

    yotta_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);

    yotta_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    yotta_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);
}

void test_wait_post()
{
    sem_init(&synchro, 0, 0);

    pthread_t producer;
    pthread_t consumer;

    yotta_sync_t yotta_sync;

    pthread_create(&producer, NULL, producer_func, (void *) &yotta_sync);
    pthread_create(&consumer, NULL, consumer_func, (void *) &yotta_sync);

    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);

    sem_destroy(&synchro);
}

void test_post_post_wait()
{
    yotta_sync_t yotta_sync;

    #pragma GCC diagnostic ignored "-Waddress"
    // This call check if the yotta_sync_t is not null
    // GCC output: warning: the comparison will always evaluate as ‘true’ for the address of ‘yotta_sync’ will never be NULL [-Waddress]
    yotta_sync_init(&yotta_sync);

    yotta_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_UNTRIGGERED);

    //TODO: not handled yet -> assert failure
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);

    yotta_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);

    yotta_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    yotta_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);
}

int main()
{
    test_post_wait();
    test_wait_post();
    /*test_post_post_wait();*/

    return 0;
}
