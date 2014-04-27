/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yotta.h>

#define print(...) fprintf(stderr, __VA_ARGS__)

// Enable/Disable output
static
uint8_t output;

// Logger entry
static
void
logger(void * user_data, yotta_logger_param_t const * param)
{
    (void) user_data;

    fprintf(stderr, "%s\n", param->msg);
}

// Struct containing the data to process for a group
typedef
struct prime_s
{
    uint64_t from;
    uint64_t to;
    yotta_addr_t master_results_addr;
}
prime_t;

// Struct containing the data to process for a thread
typedef
struct thread_prime_s
{
    prime_t * prime;
    uint8_t * results;
}
thread_prime_t;


/*
 * Tells whether or not the given number is prime.
 */
static inline
int
is_prime(uint64_t nb)
{
    if(nb < 2) return 0;
    for (size_t k = 2; k * k <= nb; k++)
        if (nb % k == 0) return 0;
    return 1;
}

/*
 * Tells whether or not the numbers in the given range are prime.
 */
static
void
is_prime_range_thread(thread_prime_t * thread_prime)
{
    uint64_t tid = 0;
    uint64_t nb_threads = 0;
    prime_t * prime = thread_prime->prime;

    yotta_get_global_id(&tid, &nb_threads);

    if(output) print("In thread %" PRIu64 " (Total: %" PRIu64 " threads)\n", tid, nb_threads);

    for (uint64_t j = prime->from + tid; j < prime->to; j += nb_threads)
    {
        if(is_prime(j))
        {
            if(output) print("%" PRIu64 " is prime (tid: %" PRIu64 ")\n", j, tid);
            yotta_atomic_fetch_or(thread_prime->results + (j - prime->from) / 8, (1u << ((j - prime->from) % 8)));
        }
    }
}

/*
 * Dispatches the prime computation to all connected slaves
 */
static
void
is_prime_range_dispatch(prime_t * prime)
{
    // Results temporary storage
    uint64_t results_size = (prime->to - prime->from) / 8 + 1;
    uint8_t * results = yotta_alloc_sa(uint8_t, results_size);
    memset(results, 0, results_size);

    thread_prime_t params = { prime, results };

    // Dispatch the computation to all available cores
    yotta_dispatch((yotta_dispatch_func_t) is_prime_range_thread, &params, 0);

    // Send the results to the master
    yotta_sync_t sync;
    yotta_push_package(prime->master_results_addr, results_size, results, &sync);
    yotta_sync_wait(&sync);

    yotta_free(results);
}


int
main(int argc, char const * const * argv)
{
    // Initialization of yotta
    yotta_context_t context;

    yotta_set_logger_entry(logger, 0);
    yotta_init(argc, argv);

    if (yotta_context_init(&context, 8000))
    {
        return 1;
    }

    // Params
    output = 1;
    uint8_t network = 1;
    uint8_t dispatch = 0;
    uint8_t massive = 0;
    uint64_t from = 0;
    uint64_t to = 0;

    for (int64_t i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--no-output") == 0)
        {
            output = 0;
            continue;
        }

        if (strcmp(argv[i], "--no-network") == 0)
        {
            network = 0;
            continue;
        }

        if (strcmp(argv[i], "--dispatch") == 0)
        {
            dispatch = 1;
            continue;
        }

        if (strcmp(argv[i], "--massive-dispatch") == 0)
        {
            massive = 1;
            dispatch = 1;
            continue;
        }

        if (strcmp(argv[i], "-f") == 0)
        {
            if(i++ < (argc - 1))
            {
                from = strtoull(argv[i], NULL, 10);
            }
            continue;
        }

        if (strcmp(argv[i], "-t") == 0)
        {
            if(i++ < (argc - 1))
            {
                to = strtoull(argv[i], NULL, 10);
            }
            continue;
        }
    }

#if 0
    print("> Parameters:\n");
    print(">> Output: %d\n", output);
    print(">> Network: %d\n", network);
    print(">> Dispatch: %d\n", dispatch);
    print(">> Massive: %d\n", massive);
    print(">> From: %" PRIu64 "\n", from);
    print(">> To: %" PRIu64 "\n", to);
    print("\n");
#endif

    // One computer no dispatch
    if(!dispatch)
    {
        print("\n### 1 COMPUTER - NO DISPATCH ###\n");
        if(output) print("\nPrime numbers from %" PRIu64 " to %" PRIu64 ": \n", from, to);
        for(uint64_t i = from; i < to; i++)
        {
            if(is_prime(i))
            {
                if(output) print("%" PRIu64 " ", i);
            }
        }
        if(output) print("\n");

        return 0;
    }


    /*yotta_context_connect(&context, "127.0.0.1", 5000);*/

    if(massive)
    {
        yotta_context_connect(&context, "192.168.0.3", 5000);
        /*yotta_context_connect(&context, "192.168.0.4", 5000);*/
        yotta_context_connect(&context, "192.168.0.5", 5000);
        yotta_context_connect(&context, "192.168.0.6", 5000);
    }

    // Results storage
    uint64_t results_size = (to - from) / 8 + 1;
    uint8_t * results = yotta_alloc_sa(uint8_t, results_size);
    memset(results, 0, results_size);

    prime_t primes;

    primes.from = from;
    primes.to = to;
    primes.master_results_addr = yotta_addr(results); // Set the results address on the master

    // Global information
    uint64_t nb_computers = 0;
    yotta_context_deamons_count(&context, &nb_computers);

    if(!network && !massive) // One computer with dispatching and no network
    {
        print("\n### 1 COMPUTER - NO NETWORK ### \n\n");

        is_prime_range_dispatch(&primes);
    }
    else // Massive dispatching via network
    {
        print("\n### %" PRIu64 " COMPUTER(S) - MASSIVE DISPATCH ### \n", nb_computers);

        // Dispatches the computation
        yotta_return_t r = yotta_context_massive(
            &context, (yotta_massive_command_entry_t) is_prime_range_dispatch,
            sizeof(prime_t), &primes, 0
        );

        if(r != YOTTA_SUCCESS)
        {
            yotta_free(results);
            return 1;
        }
    }


    // Print results
    if(output)
    {
        print("\nPrime numbers from %" PRIu64 " to %" PRIu64 ": \n", from, to);

        uint64_t u = primes.from;
        for (uint64_t i = 0; i < results_size; i++)
        {
            for (uint64_t j = 0; j < 8 && ++u <= primes.to; j++)
            {
                if((results[i] & (1u << j)))
                {
                    print("%" PRIu64 " ", primes.from + i * 8 + j);
                }
            }
        }

        print("\n");
    }

    yotta_free(results);

    if (yotta_context_destroy(&context))
    {
        return 1;
    }

    return 0;
}
