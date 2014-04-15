
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <yotta.h>


static
void
logger(void * user_data, yotta_logger_param_t const * param)
{
    (void) user_data;

    fprintf(stderr, "%s\n", param->msg);
}

static
void
is_prime(void * nb)
{
    fprintf(stderr, "In is_prime\n");
    uint64_t sqrt_nb = sqrt((uint64_t)nb) + 1;

    for (uint64_t i = 0; i < sqrt_nb; i++)
    {
        if(((uint64_t)nb) % i == 0)
        {
            fprintf(stderr, "%" PRIu64 " is not prime\n", (uint64_t) nb);
            return;
        }
    }

    fprintf(stderr, "%" PRIu64 " is prime\n", (uint64_t) nb);
}

int
main(int argc, char const * const * argv)
{
    yotta_context_t context;

    yotta_set_logger_entry(logger, 0);
    yotta_init(argc, argv);

    if (yotta_context_init(&context, 8000))
    {
        return 1;
    }

    fprintf(stderr, "Initialized\n");

    if (yotta_context_connect(&context, "127.0.0.1", 5000))
    {
        return 1;
    }

    fprintf(stderr, "Connected\n");

    uint64_t nb = 43;

    if (yotta_context_massive(&context, is_prime, 8, &nb, 0))
    {
        return 1;
    }

    fprintf(stderr, "Massived\n");

    if (yotta_context_destroy(&context))
    {
        return 1;
    }

    return 0;
}
