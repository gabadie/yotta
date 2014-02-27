#ifndef _YOTTA_PUSH
#define _YOTTA_PUSH

#include "../core/yotta_memory.h"


/*
 * @infos: pushs a memory package to the master memory
 *
 * @param <master_addr>: the master address
 * @param <buffer_size>: the buffer size
 * @param <buffer>: the buffer to push
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <master_addr>, <buffer_size>, or <buffer> is null
 *  <YOTTA_INVALID_OPERATION> if not executed on a slave
 */
uint64_t
yotta_push_package(uint64_t master_addr, uint64_t buffer_size, void const * buffer);

/*
 * @infos: pushs a memory buffer to the master memory
 *
 * @param <master_addr>: the master address
 * @param <buffer_size>: the buffer size
 * @param <buffer>: the buffer to push
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <master_addr>, or <buffer> is null
 *  <YOTTA_INVALID_OPERATION> if not executed on a slave
 *
 * @assert <buffer> allocated by the yotta library
 */
#define yotta_push_buffer(master_addr,buffer) \
    return yotta_push_package(master_addr, yotta_memory_size(buffer), buffer)


#endif
