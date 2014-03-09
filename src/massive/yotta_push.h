#ifndef _YOTTA_PUSH
#define _YOTTA_PUSH

#include "../core/yotta_addr.h"
#include "../core/yotta_memory.h"
#include "../threading/yotta_sync.h"


/*
 * @infos: pushs a memory package to the master memory
 *
 * @param <master_addr>: the master address
 * @param <buffer_size>: the buffer size
 * @param <buffer>: the buffer to push
 * @param <sync_finished>: the sync event oject triggered when the push is finished
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <master_addr>, <buffer_size>, or <buffer> is null
 */
yotta_return_t
yotta_push_package(yotta_addr_t master_addr, uint64_t buffer_size, void const * buffer, yotta_sync_t * sync);

/*
 * @infos: pushs a memory buffer to the master memory
 *
 * @param <master_addr>: the master address
 * @param <buffer_size>: the buffer size
 * @param <buffer>: the buffer to push
 * @param <sync_finished>: the sync event oject triggered when the push is finished
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <master_addr>, or <buffer> is null
 *
 * @assert <buffer> allocated by the yotta library
 */
#define yotta_push_buffer(master_addr,buffer,sync_finished) \
    return yotta_push_package(master_addr, yotta_memory_size(buffer), buffer, sync_finished)


#endif
