
#include "yotta_shared_counter.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../slave/yotta_slave_context.private.h"
#include "../threading/yotta_atomic.h"


yotta_return_t
yotta_shared_counter_init(
    yotta_shared_counter_t * shared,
    yotta_addr_t master_counter_addr,
    yotta_counter_t range_size,
    yotta_counter_t stock_size
)
{
    if (shared == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_init, shared)
    }
    else if (master_counter_addr == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_init, master_counter_addr)
    }
    else if (range_size == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_init, range_size)
    }
    else if (stock_size > range_size)
    {
        yotta_return_inv_value(yotta_shared_counter_init, stock_size)
    }

    shared->master_counter_addr = master_counter_addr;

    if (yotta_slave_context == 0)
    {
        /*
         * On the master process, only master_counter_addr maters
         */
        return YOTTA_SUCCESS;
    }

    shared->range_size = range_size;
    shared->range_status = 0;
    shared->stock_size = stock_size;

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_shared_counter_fetch_add(yotta_shared_counter_t * shared, yotta_counter_t * value, yotta_counter_t add)
{
    if (shared == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_fetch_add, shared)
    }
    else if (value == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_fetch_add, value)
    }
    else if (add == 0)
    {
        yotta_return_inv_value(yotta_shared_counter_fetch_add, add)
    }
    else if (yotta_slave_context == 0)
    {
        /*
         * On the master process, we directly use the xadd instruction
         */
        yotta_counter_t * atomic = (yotta_counter_t *) shared->master_counter_addr;

        *value = yotta_atomic_fetch_add(atomic, add);

        return YOTTA_SUCCESS;
    }

    yotta_todo("implement yotta_whisper_counter_fetch_add()");

    return YOTTA_SUCCESS;
}
