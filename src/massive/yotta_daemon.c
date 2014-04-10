
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

void
yotta_daemon_destroy(yotta_daemon_t * daemon)
{
    yotta_assert(daemon != NULL);
    yotta_assert(daemon->context != NULL);

    yotta_dictate_queue_destroy(&daemon->dictate_queue);
    yotta_whisper_queue_destroy(&daemon->whisper_queue);

    daemon->context = NULL;
}
