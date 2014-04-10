#ifndef _YOTTAPRIVATE_DICTATE_DEAMON_INFO
#define _YOTTAPRIVATE_DICTATE_DEAMON_INFO

#include "../yotta_prefix.h"
#include "yotta_dictate_queue.private.h"

/*
 * @infos: Processes a daemon info reception
 *
 * @params: see yotta_dictate_recv_t in yotta_dictate_queue.private.h
 */
void
yotta_dictate_daemon_info_recv(
    yotta_dictate_queue_t * cmd_queue
);

/*
 * Default handler of the dictate queue's vtable for daemon info reception
 */
void
yotta_dictate_vtable_daemon_info_recv(yotta_dictate_queue_t * queue, uint64_t nb_computers, uint64_t nb_threads);

#endif //_YOTTAPRIVATE_DICTATE_DEAMON_INFO
