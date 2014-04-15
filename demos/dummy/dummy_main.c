
#include <inttypes.h>
#include <stdio.h>
#include <yotta.h>

static
void
logger(void * user_data, yotta_logger_param_t const * param)
{
    (void) user_data;

    fprintf(stderr, "%s\n", param->msg);
}

typedef
struct prime_s
{
    uint64_t nb;
    yotta_addr_t master_addr;
}
prime_t;


static
void
is_prime(prime_t * prime)
{
    uint64_t nb = prime->nb;
    fprintf(stderr, "In is_prime %" PRIu64 "\n", nb);

    uint8_t res = 1;

    for (uint64_t i = 2; i * i < nb; i++)
    {
        if (nb % i == 0)
        {
            fprintf(stderr, "%" PRIu64 " is not prime\n", nb);
            res = 0;
            break;
        }
    }

    fprintf(stderr, "%" PRIu64 " is prime\n", nb);

    yotta_sync_t sync;
    yotta_push_package(prime->master_addr, 1, &res, &sync);
    yotta_sync_wait(&sync);

    fprintf(stderr, "Push DONE\n");
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

    uint8_t result = 2;

    prime_t prime;
    prime.nb = 43;
    prime.master_addr = yotta_addr(&result);

    /*is_prime(&prime);*/

    if (yotta_context_massive(&context, (yotta_massive_command_entry_t) is_prime, sizeof(prime), &prime, 0))
    {
        return 1;
    }

    fprintf(stderr, "Massived\n");

    fprintf(stderr, "%" PRIu64 " is prime ? %d\n", prime.nb, result);

    if (yotta_context_destroy(&context))
    {
        return 1;
    }

    return 0;
}
