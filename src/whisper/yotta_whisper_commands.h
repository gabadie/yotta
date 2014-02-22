#ifndef _YOTTAPRIVATE_WHISPER_COMMANDS
#define _YOTTAPRIVATE_WHISPER_COMMANDS

#include "yotta_whisper_entry.private.h"


/*
 * @infos: sends a pull request in the <socket>
 *
 * @param <socket>: the socket to send to the pull request
 * @param <master_address>: absolute address in the master memory
 * @param <data_size>: the data_size
 */
void
yotta_whisper_pull(
    yotta_socket_t * socket,
    uint64_t master_address,
    uint64_t size,
    void * dest
);

/*
 * @infos: pushes <data> in the <socket>
 *
 * @param <socket>: the socket to send to the pull request
 * @param <master_address>: absolute address in the master memory
 * @param <data_size>: the data size
 * @param <data>: the data to push
 */
void
yotta_whisper_push(
    yotta_socket_t * socket,
    uint64_t master_address,
    uint64_t data_size,
    void * const data
);

#endif
