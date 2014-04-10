
#include <yotta.h>


void
launch(yotta_context_t * context)
{
    (void) context;
}

int
main(int argc, char const * const * argv)
{
    yotta_init(argc, argv);

    yotta_context_t context;

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
