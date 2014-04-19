#ifndef _YOTTAPRIVATE_DICTATE_UNKNOWN
#define _YOTTAPRIVATE_DICTATE_UNKNOWN

#include "../yotta_prefix.h"
#include "yotta_dictate_queue.private.h"

/*
 * @infos: Processes a unknown (or depracated) frame
 *
 * @params: see yotta_dictate_recv_t in yotta_dictate_queue.private.h
 */
void
yotta_dictate_unknown_recv(
    yotta_dictate_queue_t * cmd_queue
);

/*
 * Default handler of the dictate queue's vtable for unknown (or depracated) frame reception
 *
 * @param label Label of the unknown or depracted frame
 * @param data_size Data size of the unknown frame
 */
void
yotta_dictate_vtable_unknown_recv(yotta_dictate_queue_t * queue);

#endif //_YOTTAPRIVATE_DICTATE_UNKNOWN
