

#include "yotta_whisper_master.h"
#include "yotta_whisper_queue.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_memory.h"
#include "../socket/yotta_socket_thread.h"
#include "../socket/yotta_tcp.h"


static
void
yotta_whisper_master_recv(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);
    yotta_assert(master->vtable != NULL);

    yotta_logger_debug("yotta_whisper_master_recv: new slave connection");

    (master->vtable->create_queue)(master);
}

static
void
yotta_whisper_master_except(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);

    yotta_logger_error("yotta_whisper_master_except: received a TCP socket exception -> releasing");

    yotta_socket_event_release(master);
}

yotta_return_t
yotta_whisper_master_init(
    yotta_whisper_master_t * master,
    yotta_whisper_master_vtable_t const * vtable,
    uint16_t listening_port)
{
    yotta_assert(master != NULL);
    yotta_assert(vtable != NULL);
    yotta_assert(vtable->create_queue != NULL);

    yotta_dirty_s(master);

    if (yotta_tcp_socket_server(&master->socket_event.socket, listening_port) != 0)
    {
        return -1;
    }

    yotta_socket_event_init(master);
    yotta_socket_event_set_except(master, yotta_whisper_master_except);
    yotta_socket_event_set_recv(master, yotta_whisper_master_recv);
    yotta_socket_event_set_send(master, 0);

#ifdef YOTTA_ASSERT
    yotta_socket_event_set_release(master, 0);
#endif //YOTTA_ASSERT

    master->vtable = vtable;

    return 0;
}

yotta_return_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master)
{
    yotta_assert(master != NULL);

    return yotta_socket_event_destroy(master);
}
