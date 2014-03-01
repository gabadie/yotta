#include <yotta.h>
#include <mk_test.h>
#include "../src/yotta_sync.private.h"

#include <pthread.h>
#include <semaphore.h>


sem_t
synchro;

void
producer_func(void * s)
{
    yotta_sync_t * yotta_sync = (yotta_sync_t *) s;

    yotta_sync_init(yotta_sync);

    test_assert(yotta_sync->sem == (sem_t *) YOTTA_SYNC_UNTRIGGERED);

    sem_post(&synchro);

    test_assert(yotta_sync_wait(yotta_sync) == YOTTA_SUCCESS);

    /*test_assert(yotta_sync->sem == (sem_t *) YOTTA_SYNC_TRIGGERED);*/
}

void
consumer_func(void * s)
{
    sem_wait(&synchro);

    yotta_sync_t * yotta_sync = (yotta_sync_t *) s;

    // To "ensure" that the yotta_sync_post happens after the yotta_sync_wait
    while(yotta_sync->sem == NULL) sleep(1);

    yotta_sync_post(yotta_sync);
}

void
test_post_wait()
{
    yotta_sync_t yotta_sync;

    #pragma GCC diagnostic ignored "-Waddress"
    // This call check if the yotta_sync_t is not null
    // GCC output: warning: the comparison will always evaluate as ‘true’ for the address of ‘yotta_sync’ will never be NULL [-Waddress]
    yotta_sync_init(&yotta_sync);

    test_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_UNTRIGGERED);

    yotta_sync_post(&yotta_sync);

    test_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);

    test_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);
}

void
test_wait_post()
{
    sem_init(&synchro, 0, 0);

    pthread_t producer;
    pthread_t consumer;

    yotta_sync_t yotta_sync;

    pthread_create(&producer, NULL, (void *) producer_func, (void *) &yotta_sync);
    pthread_create(&consumer, NULL, (void *) consumer_func, (void *) &yotta_sync);

    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);

    sem_destroy(&synchro);
}

void
test_post_post_wait()
{
    yotta_sync_t yotta_sync;

    #pragma GCC diagnostic ignored "-Waddress"
    // This call check if the yotta_sync_t is not null
    // GCC output: warning: the comparison will always evaluate as ‘true’ for the address of ‘yotta_sync’ will never be NULL [-Waddress]
    yotta_sync_init(&yotta_sync);

    test_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_UNTRIGGERED);

    //TODO: not handled yet -> assert failure
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);

    test_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);

    test_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync.sem == (sem_t *) YOTTA_SYNC_TRIGGERED);
}

int
main()
{
    /*test_post_wait();*/
    test_wait_post();
    /*test_post_post_wait();*/

    return 0;
}
