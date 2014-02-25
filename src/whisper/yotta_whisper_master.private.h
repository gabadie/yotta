#ifndef _YOTTAPRIVATE_WHISPER_MASTER
#define _YOTTAPRIVATE_WHISPER_MASTER

#include "../socket/yotta_socket_event.h"


/*
 * @infos: defines the whisper master event
 */
typedef
yotta_socket_event_t
yotta_whisper_master_t;

/*
 * @infos: initializes the whisper master. the release event would have to be set
 *  with yotta_socket_event_set_release() (yotta_socket_event.h)
 *
 * @param <master>: the master to initialize
 * @param <listening_port>: the master's listening port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_whisper_master_init(yotta_whisper_master_t * master, uint16_t listening_port);

/*
 * @infos: destroyes the whisper master
 *
 * @param <master>: the master to destroyes
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master);

#endif
