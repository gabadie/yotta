
#include <string.h>

#include "yotta_command.h"
#include "core/yotta_return.private.h"


uint64_t
yotta_command_init(yotta_command_t * command)
{
    if (command == 0)
    {
        yotta_return_inv_value(yotta_command_init, command);
    }

    memset(command, 0, sizeof(yotta_command_t));

    return YOTTA_SUCCESS;
}

uint64_t
yotta_command(const yotta_context_t * context, const yotta_command_t * command)
{
    if (context == 0)
    {
        yotta_return_inv_value(yotta_command, context);
    }
    else if (command == 0)
    {
        yotta_return_inv_value(yotta_command, command);
    }

    // TODO: code the content

    return YOTTA_SUCCESS;
}
