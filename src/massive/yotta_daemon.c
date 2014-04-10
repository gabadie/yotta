
#include "yotta_daemon.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"


yotta_return_t
yotta_daemon_init(yotta_daemon_t * daemon, yotta_context_t * context, char const * ip, uint16_t port)
{
    yotta_assert(daemon != NULL);
    yotta_assert(context != NULL);
    yotta_assert(ip != NULL);
    yotta_assert(port != 0);

    (void) daemon;
    (void) context;
    (void) ip;
    (void) port;

    yotta_todo("implement here");

    return YOTTA_UNEXPECTED_FAIL;
}

yotta_return_t
yotta_daemon_destroy(yotta_daemon_t * daemon)
{
    yotta_assert(daemon != NULL);

    (void) daemon;

    yotta_todo("implement here");

    return YOTTA_UNEXPECTED_FAIL;
}
