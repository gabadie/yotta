
#include <inttypes.h>
#include <stdio.h>
#include <yotta.h>

#define RANGE_SIZE 10

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
    uint64_t range;
    yotta_addr_t master_addr;
}
prime_t;

static inline
int
is_prime(uint64_t nb)
{
    if(nb < 2) return 0;
    for (size_t k = 2; k * k <= nb; k++)
        if (nb % k == 0) return 0;
    return 1;
}

static
void
is_prime_range(prime_t * prime)
{
    uint8_t * results = yotta_alloc_sa(uint8_t, prime->range);
    memset(results, 0, prime->range);

    for(uint64_t i = 0; i < prime->range; i++)
    {
        for (uint64_t j = 0; j < 8; j++)
        {
            uint64_t nb = prime->nb + i * 8 + j;

            if(is_prime(nb))
            {
                results[i] |= (1u << j);
                fprintf(stderr, "%" PRIu64 " is prime\n", nb);
            }
            else
            {
                fprintf(stderr, "%" PRIu64 " is not prime\n", nb);
            }
        }
        fprintf(stderr, "%d\n", results[i]);
    }

    // Send the results to the master
    yotta_sync_t sync;
    yotta_push_package(prime->master_addr, prime->range, results, &sync);
    yotta_sync_wait(&sync);

    fprintf(stderr, "Push DONE\n");

    yotta_free(results);
}

typedef
struct thread_prime_s
{
    prime_t * prime;
    uint8_t * results;
}
thread_prime_t;

static
void
thread_is_prime(thread_prime_t * thread_prime)
{
    uint64_t tid = 0;
    uint64_t nb_threads = 0;
    prime_t * prime = thread_prime->prime;

    yotta_get_local_id(&tid, &nb_threads);

    /*fprintf(stderr, "In thread %" PRIu64 " (%" PRIu64 " threads)\n", tid, nb_threads);*/

    for (uint64_t j = tid; j < prime->range * 8; j += nb_threads)
    {
        if(is_prime(prime->nb + j))
        {
            /*fprintf(stderr, "%" PRIu64 " is prime (tid: %" PRIu64 ")\n", prime->nb + j, tid);*/
            yotta_atomic_fetch_or(thread_prime->results + j / 8, (1u << (j % 8)));
        }
    }
}

static
void
is_prime_range_dispatch(prime_t * prime)
{
    uint8_t * results = yotta_alloc_sa(uint8_t, prime->range);
    memset(results, 0, prime->range);

    thread_prime_t params;
    params.prime = prime;
    params.results = results;

    // Dispatch the computation to all available cores
    yotta_dispatch((yotta_dispatch_func_t) thread_is_prime, &params, 0);

    /*fprintf(stderr, "Dispatch DONE\n");*/

    // Send the results to the master
    yotta_sync_t sync;
    yotta_push_package(prime->master_addr, prime->range, results, &sync);
    yotta_sync_wait(&sync);

    /*fprintf(stderr, "Push DONE\n");*/

    yotta_free(results);
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

    if (yotta_context_connect(&context, "127.0.0.1", 5000))
    {
        return 1;
    }

    fprintf(stderr, "Connected\n");

    uint8_t results[RANGE_SIZE];
    memset(results, 1, RANGE_SIZE);

    prime_t prime;

    prime.nb = 2;
    prime.range = RANGE_SIZE;
    prime.master_addr = yotta_addr(&results);

    /*is_prime_range_dispatch(&prime);*/

    if (yotta_context_massive(&context, (yotta_massive_command_entry_t) is_prime_range, sizeof(prime), &prime, 0))
    {
        return 1;
    }

    fprintf(stderr, "Massived\n");

    for (size_t i = 0; i < prime.range; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            fprintf(stderr, "%" PRIu64 " is prime: %d\n", prime.nb + i * 8 + j, (results[i] & (1u << j)) >> j);
        }
    }

    if (yotta_context_destroy(&context))
    {
        return 1;
    }

    return 0;
}
