
#include "yotta_push.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.private.h"
#include "../slave/yotta_slave_context.private.h"
#include "../whisper/yotta_whisper_push.private.h"


uint64_t
yotta_push_package(uint64_t master_addr, uint64_t buffer_size, void const * buffer, yotta_sync_t * sync_finished)
{
    if (master_addr == 0)
    {
        yotta_return_inv_value(yotta_push_package, master_addr);
    }
    else if (buffer_size == 0)
    {
        yotta_return_inv_value(yotta_push_package, buffer_size);
    }
    else if (buffer == 0)
    {
        yotta_return_inv_value(yotta_push_package, buffer);
    }
    else if (yotta_slave_context == 0)
    {
        yotta_return_inv_op(yotta_push_package);
    }

    yotta_whisper_push(&yotta_slave_context->queue, master_addr, buffer_size, buffer, sync_finished);

    return YOTTA_SUCCESS;
}
