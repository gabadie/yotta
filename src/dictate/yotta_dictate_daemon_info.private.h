#ifndef _YOTTAPRIVATE_DICTATE_DEAMON_INFO
#define _YOTTAPRIVATE_DICTATE_DEAMON_INFO

/*
 * @infos: processes a command feedback
 *
 * @params: see yotta_dictate_recv_t in yotta_dictate_queue.private.h
 */
void
yotta_dictate_daemon_info_recv(
    yotta_dictate_queue_t * cmd_queue
);

#endif //_YOTTAPRIVATE_DICTATE_DEAMON_INFO
