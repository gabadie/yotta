#include <stdlib.h>

#include <yotta.h>
#include <mk_test.h>

#include "../src/yotta_debug.h"
#include "../src/yotta_logger.h"

static int challenge = 0;

void *
foo(void * _)
{
    (void) _;
    challenge = 42;
    return NULL;
}

int
main(int argc, char const * const * argv)
{
    yotta_init(argc, argv);

    yotta_assert(challenge == 0);

    yotta_socket_thread_t thread;

    yotta_socket_thread_init(&thread, &foo, NULL);

    yotta_socket_thread_join(&thread);

    yotta_assert(challenge == 42);

    return 0;
}


