
#include "yotta_daemon.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../dictate/yotta_dictate_daemon_error.private.h"
#include "../dictate/yotta_dictate_unknown.private.h"
#include "../threading/yotta_atomic.h"


void
yotta_daemon_daemon_info_process(yotta_dictate_queue_t * queue, uint64_t nb_computers, uint64_t nb_threads)
{
    yotta_assert(queue != NULL);
    yotta_assert(nb_computers != 0);
    yotta_assert(nb_threads != 0);

    yotta_daemon_t * daemon = (yotta_daemon_t *) queue;

    yotta_assert((daemon->status & YOTTA_DAEMON_STATUS_READY) == 0x0);

    daemon->available_computers = nb_computers;
    daemon->available_threads = nb_threads;

    yotta_atomic_fetch_or(&daemon->status, YOTTA_DAEMON_STATUS_READY);
}

static
yotta_dictate_vtable_t const
yotta_daemon_dictate_vtable =
{
    yotta_daemon_daemon_info_process,
    yotta_dictate_vtable_daemon_error_recv,
    yotta_dictate_vtable_unknown_recv
};

yotta_return_t
yotta_daemon_init(yotta_daemon_t * daemon, yotta_context_t * context, char const * ip, uint16_t port)
{
    yotta_assert(daemon != NULL);
    yotta_assert(daemon->context != NULL);
    yotta_assert(context != NULL);
    yotta_assert(ip != NULL);
    yotta_assert(port != 0);

    if (yotta_dictate_queue_connect(&daemon->dictate_queue, ip, port))
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    daemon->dictate_queue.vtable = &yotta_daemon_dictate_vtable;
    daemon->context = context;
    daemon->status = 0x0;

    return YOTTA_SUCCESS;
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
