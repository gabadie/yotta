

#include "yotta_whisper_master.h"
#include "yotta_whisper_queue.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_memory.h"
#include "../socket/yotta_socket_thread.h"
#include "../socket/yotta_tcp.h"


static
void
yotta_whisper_queue_release(yotta_whisper_queue_t * cmd_queue)
{
    yotta_assert(cmd_queue != 0);

    yotta_whisper_queue_destroy(cmd_queue);

    yotta_free(cmd_queue);
}

static
void
yotta_whisper_master_recv(yotta_whisper_master_t * master)
{
    yotta_assert(master != 0);

    yotta_logger_debug("yotta_whisper_master_recv: new slave connection");

    yotta_socket_t slave_socket;

    yotta_whisper_queue_t * cmd_queue = yotta_alloc_s(yotta_whisper_queue_t);

    yotta_dirty_s(cmd_queue);

    if (yotta_socket_accept(&master->socket, &slave_socket) != 0)
    {
        yotta_logger_debug("yotta_whisper_master_recv: connection has failed");
        return;
    }

    yotta_whisper_queue_init(cmd_queue);
    yotta_socket_event_set_release(cmd_queue, yotta_whisper_queue_release);

    if (yotta_socket_thread_listen(master->socket_thread, (yotta_socket_event_t *) cmd_queue) != 0)
    {
        yotta_whisper_queue_destroy(cmd_queue);
        yotta_free(cmd_queue);
    }
}

static
void
yotta_whisper_master_except(yotta_whisper_master_t * master)
{
    yotta_assert(master != 0);

    yotta_logger_error("yotta_whisper_master_except: received a TCP socket exception -> releasing");

    yotta_socket_event_release(master);
}

yotta_return_t
yotta_whisper_master_init(yotta_whisper_master_t * master, uint16_t listening_port)
{
    yotta_assert(master != 0);

    yotta_dirty_s(master);

    if (yotta_tcp_socket_server(&master->socket, listening_port) != 0)
    {
        return -1;
    }

    yotta_socket_event_init(master);
    yotta_socket_event_set_except(master, yotta_whisper_master_except);
    yotta_socket_event_set_recv(master, yotta_whisper_master_recv);
    yotta_socket_event_set_send(master, 0);

#ifdef YOTTA_DEBUG
    yotta_socket_event_set_release(master, 0);
#endif // YOTTA_DEBUG

    return 0;
}

yotta_return_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master)
{
    yotta_assert(master != 0);

    return yotta_socket_event_destroy(master);
}
