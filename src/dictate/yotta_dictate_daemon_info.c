#include <stdio.h>
#include <inttypes.h>

#include "yotta_dictate_daemon_info.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"

void
yotta_dictate_daemon_info_recv(
    yotta_dictate_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != NULL);
    yotta_assert(cmd_queue->vtable != NULL);

    (void) cmd_queue;
    typedef struct
    yotta_dictate_buffer_s
    {
        uint64_t header_cursor;
        struct
        {

        };
    } yotta_dictate_buffer_t;
}

void
yotta_dictate_vtable_daemon_info_recv(uint64_t nb_computers, uint64_t nb_threads)
{
    fprintf(stderr, "Nb computers: %" PRIu64 " | nb threads: %" PRIu64"", nb_computers, nb_threads);
}
