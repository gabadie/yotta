
#include <string.h>

#include "yotta_push.h"
#include "../core/yotta_return.private.h"
#include "../slave/yotta_slave_context.private.h"
#include "../whisper/yotta_whisper_fetch.private.h"


yotta_return_t
yotta_fetch_package(yotta_addr_t master_addr, uint64_t buffer_size, void * dest_buffer, yotta_sync_t * sync_finished)
{
    if (master_addr == 0)
    {
        yotta_return_inv_value(yotta_push_package, master_addr);
    }
    else if (buffer_size == 0)
    {
        yotta_return_inv_value(yotta_push_package, buffer_size);
    }
    else if (dest_buffer == 0)
    {
        yotta_return_inv_value(yotta_push_package, dest_buffer);
    }
    else if (sync_finished == 0)
    {
        yotta_return_inv_value(yotta_push_package, sync_finished);
    }
    else if (yotta_slave_context == 0)
    {
        memcpy(dest_buffer, (void *) master_addr, buffer_size);

        yotta_sync_init(sync_finished);
        yotta_sync_post(sync_finished);

        return YOTTA_SUCCESS;
    }

    yotta_whisper_fetch(&yotta_slave_context->queue, master_addr, buffer_size, dest_buffer, sync_finished);

    return YOTTA_SUCCESS;
}
