
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

typedef
struct thread_prime_s
{
    prime_t * prime;
    uint8_t * results;
}
thread_prime_t;

static
void
is_prime_range_thread(thread_prime_t * thread_prime)
{
    uint64_t tid = 0;
    uint64_t nb_threads = 0;
    prime_t * prime = thread_prime->prime;

    yotta_get_local_id(&tid, &nb_threads);

    fprintf(stderr, "In thread %" PRIu64 " (%" PRIu64 " threads)\n", tid, nb_threads);

    for (uint64_t j = tid; j < prime->range * 8; j += nb_threads)
    {
        if(is_prime(prime->nb + j))
        {
            fprintf(stderr, "%" PRIu64 " is prime (tid: %" PRIu64 ")\n", prime->nb + j, tid);
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

    thread_prime_t params = { prime, results };

    // Dispatch the computation to all available cores
    yotta_dispatch((yotta_dispatch_func_t) is_prime_range_thread, &params, 0);

    fprintf(stderr, "Dispatch DONE\n");

    // Send the results to the master
    yotta_sync_t sync;
    yotta_push_package(prime->master_addr, prime->range, results, &sync);
    yotta_sync_wait(&sync);

    fprintf(stderr, "Push DONE\n");

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

#if 0
    if (yotta_context_connect(&context, "127.0.0.1", 5001))
    {
        return 1;
    }
#endif

    fprintf(stderr, "Connected\n");

    uint64_t const NB_DAEMONS = 2;

    uint8_t results[NB_DAEMONS * RANGE_SIZE];
    memset(results, 1, NB_DAEMONS * RANGE_SIZE);

    prime_t primes[NB_DAEMONS];

    primes[0].nb = 2;
    primes[0].range = RANGE_SIZE;
    primes[0].master_addr = yotta_addr(results);

    primes[1].nb = 2 + 8 * RANGE_SIZE;
    primes[1].range = RANGE_SIZE;
    primes[1].master_addr = yotta_addr(results + RANGE_SIZE);

    /*is_prime_range_dispatch(&primes[0]);*/
    /*is_prime_range_dispatch(&primes[1]);*/

    if (
        yotta_context_massive(
            &context,
            (yotta_massive_command_entry_t) is_prime_range_dispatch,
            sizeof(prime_t),
            &primes[0],
            0
            /*sizeof(prime_t)*/
        )
    )
    {
        return 1;
    }

    if (
        yotta_context_massive(
            &context,
            (yotta_massive_command_entry_t) is_prime_range_dispatch,
            sizeof(prime_t),
            &primes[1],
            0
            /*sizeof(prime_t)*/
        )
    )
    {
        return 1;
    }

    fprintf(stderr, "Massived\n");

    for( size_t p = 0; p < NB_DAEMONS; p++)
    {
        size_t offset = p * primes[p].range;
        for (size_t i = 0; i < primes[p].range; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                fprintf(stderr, "%" PRIu64 " is prime: %d\n",
                    primes[p].nb + i * 8 + j, (results[offset + i] & (1u << j)) >> j);
            }
        }
    }

    if (yotta_context_destroy(&context))
    {
        return 1;
    }

    return 0;
}
