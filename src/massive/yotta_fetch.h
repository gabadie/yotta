#ifndef _YOTTA_FETCH
#define _YOTTA_FETCH

#include "../core/yotta_addr.h"
#include "../threading/yotta_sync.h"


/*
 * @infos: fetches a memory package from the master memory
 *
 * @param <master_addr>: the master address
 * @param <buffer_size>: the buffer size
 * @param <buffer_dest>: the buffer destination
 * @param <sync_finished>: the sync event oject triggered when the push is finished
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <master_addr>, <buffer_size>, <buffer_dest> or <sync> are null
 *  <YOTTA_INVALID_OPERATION> if not executed on a slave
 */
yotta_return_t
yotta_fetch_package(yotta_addr_t master_addr, uint64_t buffer_size, void * buffer_dest, yotta_sync_t * sync);


#endif
