
#include <pthread.h>
#include <yotta.h>
#include <mk_test.h>
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

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress"
    /*
     * This call check if the yotta_sync_t is not null
     * GCC output: warning: the comparison will always evaluate as ‘true’ for
     * the address of ‘yotta_sync’ will never be NULL [-Waddress]
     */
    yotta_sync_init(&yotta_sync);
    #pragma GCC diagnostic pop

    test_assert(yotta_sync.sem == YOTTA_SYNC_UNTRIGGERED);

    yotta_sync_post(&yotta_sync);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);

    test_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);
}

void
test_wait_post()
{
    pthread_t producer;
    pthread_t consumer;

    yotta_sync_t yotta_sync;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress"
    /*
     * This call check if the yotta_sync_t is not null
     * GCC output: warning: the comparison will always evaluate as ‘true’ for
     * the address of ‘yotta_sync’ will never be NULL [-Waddress]
     */
    yotta_sync_init(&yotta_sync);
    #pragma GCC diagnostic pop

    pthread_create(&producer, NULL, (void *) producer_func, (void *) &yotta_sync);
    pthread_create(&consumer, NULL, (void *) consumer_func, (void *) &yotta_sync);

    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
}

void
test_post_post_wait()
{
    yotta_sync_t yotta_sync;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress"
    /*
     * This call check if the yotta_sync_t is not null
     * GCC output: warning: the comparison will always evaluate as ‘true’ for
     * the address of ‘yotta_sync’ will never be NULL [-Waddress]
     */
    yotta_sync_init(&yotta_sync);
    #pragma GCC diagnostic pop

    test_assert(yotta_sync.sem == YOTTA_SYNC_UNTRIGGERED);

    //TODO: not handled yet -> assert failure
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);
    yotta_sync_post(&yotta_sync);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);

    test_assert(yotta_sync_wait(&yotta_sync) == YOTTA_SUCCESS);

    test_assert(yotta_sync.sem == YOTTA_SYNC_TRIGGERED);
}

int
main()
{
    test_post_wait();
    test_wait_post();
    /*test_post_post_wait();*/

    return 0;
}
