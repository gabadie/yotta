
#include "yotta_context.h"
#include "yotta_daemon.private.h"
#include "../yotta_init.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../dictate/yotta_dictate_binary_send.private.h"
#include "../dictate/yotta_dictate_daemon_error.private.h"
#include "../dictate/yotta_dictate_daemon_start.private.h"
#include "../dictate/yotta_dictate_unknown.private.h"
#include "../threading/yotta_atomic.h"
#include "../threading/yotta_sync.private.h"


void
yotta_daemon_daemon_info_process(yotta_dictate_queue_t * queue, uint64_t nb_computers, uint64_t nb_threads)
{
    yotta_assert(queue != NULL);
    yotta_assert(nb_computers != 0);
    yotta_assert(nb_threads != 0);

    yotta_daemon_t * daemon = (yotta_daemon_t *) queue;

    yotta_assert((daemon->status & YOTTA_DAEMON_DICTATE_READY) == 0x0);

    daemon->available_computers = nb_computers;
    daemon->available_threads = nb_threads;

    yotta_atomic_fetch_or(&daemon->status, YOTTA_DAEMON_DICTATE_READY);

    yotta_sync_post(&daemon->sync_dictate_ready);
}

static
yotta_dictate_vtable_t const
yotta_daemon_dictate_vtable =
{
    yotta_daemon_daemon_info_process,
    yotta_dictate_vtable_daemon_error_recv,
    yotta_dictate_vtable_unknown_recv
};

static
void
yotta_daemon_dictate_release(yotta_tcp_queue_t * dictate_queue)
{
    yotta_daemon_t * daemon = (yotta_daemon_t *) dictate_queue;

    if (daemon->status & YOTTA_DAEMON_DICTATE_READY)
    {
        yotta_dictate_queue_destroy(&daemon->dictate_queue);
    }

    if (daemon->status & YOTTA_DAEMON_WHISPER_READY)
    {
        yotta_whisper_queue_destroy(&daemon->whisper_queue);
    }
}

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

    yotta_socket_event_set_release(&daemon->dictate_queue, yotta_daemon_dictate_release);

    yotta_sync_init(&daemon->sync_dictate_ready);
    yotta_sync_init(&daemon->sync_whisper_ready);

    yotta_socket_thread_listen(&context->worker_thread, (yotta_socket_event_t *) &daemon->dictate_queue);

    daemon->dictate_queue.vtable = &yotta_daemon_dictate_vtable;
    daemon->context = context;
    daemon->status = 0x0;

    uint16_t whisper_port = 0;

    yotta_socket_host(&daemon->context->whisper_master.socket_event.socket, NULL, &whisper_port);

    yotta_dictate_binary(&daemon->dictate_queue, yotta_executable_path, NULL);
    yotta_dictate_daemon_start(&daemon->dictate_queue, whisper_port, NULL);

    yotta_sync_wait(&daemon->sync_dictate_ready);
    yotta_sync_wait(&daemon->sync_whisper_ready);

    return YOTTA_SUCCESS;
}

void
yotta_daemon_destroy(yotta_daemon_t * daemon)
{
    yotta_assert(daemon != NULL);
    yotta_assert(daemon->context != NULL);

    yotta_tcp_queue_finish((yotta_tcp_queue_t *) &daemon->dictate_queue);

    if (daemon->status & YOTTA_DAEMON_WHISPER_READY)
    {
        yotta_tcp_queue_finish((yotta_tcp_queue_t *)&daemon->whisper_queue);
    }

    daemon->context = NULL;
}
