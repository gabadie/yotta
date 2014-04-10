
#include <stdio.h>
#include "../../src/yotta.h"


static
void
logger(void * user_data, yotta_logger_param_t const * param)
{
    (void) user_data;

    fprintf(stderr, "%s\n", param->msg);
}

static
void
launch(yotta_context_t * context)
{
    (void) context;
}

int
main(int argc, char const * const * argv)
{
    yotta_context_t context;

    yotta_set_logger_entry(logger, 0);
    yotta_init(argc, argv);

    if (yotta_context_init(&context, 0))
    {
        return 1;
    }

    if (yotta_context_connect(&context, "127.0.0.1", 5000))
    {
        return 1;
    }

    launch(&context);

    if (yotta_context_destroy(&context))
    {
        return 1;
    }

    return 0;
}
