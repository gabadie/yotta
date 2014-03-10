
#include <pthread.h>

#include "testhelper_memory.h"
#include "../src/threading/yotta_sync.private.h"

#ifdef YOTTA_UNIX
#include <unistd.h>
#endif


void
producer_func(void * s)
{
    yotta_sync_t * yotta_sync = (yotta_sync_t *) s;

    test_assert(yotta_sync->sem == YOTTA_SYNC_UNTRIGGERED);

    test_assert(yotta_sync_wait(yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync->sem == YOTTA_SYNC_TRIGGERED);
}

void
consumer_func(void * s)
{
    yotta_sync_t * yotta_sync = (yotta_sync_t *) s;

    // To "ensure" that the yotta_sync_post happens after the yotta_sync_wait
    while(yotta_sync->sem == NULL)
    {
#ifdef YOTTA_UNIX
        test_assert(usleep(10000) == 0);
#else
        sleep(1);
#endif
    }

    yotta_sync_post(yotta_sync);
}

void
test_post_wait()
{
    yotta_sync_t yotta_sync;

    yotta_sync_init(&yotta_sync);

    test_assert(yotta_sync.sem == YOTTA_SYNC_UNTRIGGERED);

    yotta_sync_post(&yotta_sync);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);

    test_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);

    testhelper_memory_check();
}

void
test_wait_post()
{
    pthread_t producer;
    pthread_t consumer;

    yotta_sync_t yotta_sync;

    yotta_sync_init(&yotta_sync);

    pthread_create(&producer, NULL, (void *) producer_func, (void *) &yotta_sync);
    pthread_create(&consumer, NULL, (void *) consumer_func, (void *) &yotta_sync);

    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);

    testhelper_memory_check();
}

void
test_post_post_wait()
{
    yotta_sync_t yotta_sync;

    yotta_sync_init(&yotta_sync);

    test_assert(yotta_sync.sem == YOTTA_SYNC_UNTRIGGERED);

    //TODO: not handled yet -> assert failure
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);

    test_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);

    testhelper_memory_check();
}

int
main()
{
    testhelper_memory_setup();

    test_post_wait();
    test_wait_post();
    /*test_post_post_wait();*/

    return 0;
}
