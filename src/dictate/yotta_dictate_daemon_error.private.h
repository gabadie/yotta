#ifndef _YOTTAPRIVATE_DICTATE_ERROR
#define _YOTTAPRIVATE_DICTATE_ERROR

#include "../yotta_prefix.h"
#include "yotta_dictate_queue.private.h"

/*
 * @infos: Processes a daemon error reception
 *
 * @params: see yotta_dictate_recv_t in yotta_dictate_queue.private.h
 */
void
yotta_dictate_daemon_error_recv(
    yotta_dictate_queue_t * cmd_queue
);

/*
 * Default handler of the dictate queue's vtable for daemon error reception
 *
 * @param msg Error message
 */
void
yotta_dictate_vtable_daemon_error_recv(char const * msg);

#endif //_YOTTAPRIVATE_DICTATE_ERROR
