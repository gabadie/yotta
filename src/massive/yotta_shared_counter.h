#ifndef _YOTTA_SHARED_COUNTER
#define _YOTTA_SHARED_COUNTER

#include "../core/yotta_addr.h"
#include "../threading/yotta_semaphore.h"


/*
 * Defines the yotta counter type
 */
typedef uint64_t yotta_counter_t;

/*
 * Defines the yotta counter client to have platform shared atomic counter
 */
typedef struct
yotta_shared_counter_s
{
    // the master address of the yotta_counter_t
    yotta_addr_t master_counter_addr;

    // the current cursor
    yotta_counter_t cursor;

    // the two ranges' starts
    yotta_counter_t range_start[2];

    // the ranges size
    yotta_counter_t range_size;

    // the ranges status bitfield
    size_t range_status;

    // the minimum stock size before to trigger a new atomic range request
    yotta_counter_t stock_size;

    // the waiting semaphore
    yotta_semaphore_t * waiting_semaphore;
}
yotta_shared_counter_t;

/*
 * @infos: Inits the shared counter
 *
 * @param <shared>: the shared counter to init
 * @param <master_counter_addr>: the counter's address on the master process
 * @param <range_size>: the request range size
 * @param <stock_size>: the minimum stock size before to trigger a new atomic
 *      rang request
 */
yotta_return_t
yotta_shared_counter_init(
    yotta_shared_counter_t * shared,
    yotta_addr_t master_counter_addr,
    yotta_counter_t range_size,
    yotta_counter_t stock_size
);

/*
 * @infos: Fetches and adds to the shared counter
 *
 * @param <shared>: the shared counter
 * @param <value>: the counter value
 * @param <add>: the value to add
 */
yotta_return_t
yotta_shared_counter_fetch_add(
    yotta_shared_counter_t * shared,
    yotta_counter_t * value,
    yotta_counter_t add
);


#endif //_YOTTA_SHARED_COUNTER
