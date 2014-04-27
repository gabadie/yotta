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
    while(yotta_sync->sem == YOTTA_SYNC_UNTRIGGERED)
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
