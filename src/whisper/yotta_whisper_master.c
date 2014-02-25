
#include "yotta_whisper_master.private.h"
#include "../socket/yotta_tcp.h"
#include "../yotta_logger.private.h"


static
void
yotta_whisper_master_recv(yotta_whisper_master_t * master)
{
    yotta_logger_debug("yotta_whisper_master_recv: new slave connection");

    // TODO: yotta_whisper_queue
    yotta_socket_t slave_socket;

    yotta_accept_socket(&master->socket, &slave_socket);
    yotta_close_socket(&slave_socket);
}

static
void
yotta_whisper_master_except(yotta_whisper_master_t * master)
{
    yotta_logger_error("yotta_whisper_master_except: received a TCP socket exception -> releasing");

    yotta_socket_event_release(master);
}

uint64_t
yotta_whisper_master_init(yotta_whisper_master_t * master, uint16_t listening_port)
{
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

uint64_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master)
{
    return yotta_socket_event_destroy(master);
}
