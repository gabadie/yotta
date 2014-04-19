#ifndef _YOTTAPRIVATE_WHISPER_MASTER
#define _YOTTAPRIVATE_WHISPER_MASTER

#include "../socket/yotta_socket_event.h"


/*
 * Forward declaration of yotta_whisper_master_t
 */
typedef
struct yotta_whisper_master_s
yotta_whisper_master_t;

/*
 * @infos: defines a whisper master's virtual table
 */
typedef
struct yotta_whisper_master_vtable_s
{
    // creates the yotta_whisper_queue
    void (*create_queue)(yotta_whisper_master_t *);
}
yotta_whisper_master_vtable_t;

/*
 * @infos: defines the whisper master event
 */
struct yotta_whisper_master_s
{
    // inherits from yotta_socket_event_t
    yotta_socket_event_t socket_event;

    // yotta_whistper master's vtable
    yotta_whisper_master_vtable_t const * vtable;
};

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
yotta_return_t
yotta_whisper_master_init(
    yotta_whisper_master_t * master,
    yotta_whisper_master_vtable_t const * vtable,
    uint16_t listening_port
);

/*
 * @infos: destroyes the whisper master
 *
 * @param <master>: the master to destroyes
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_whisper_master_destroy(yotta_whisper_master_t * master);

#endif
