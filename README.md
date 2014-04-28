Yotta Distributed Computing Library
===================================

## Introduction

Yotta Distributed Computing Library, named from the International System of Units (SI) denoting a factor of 10^24, is a C library designed for network-based distributed computing. Yotta provides features to the developer in order to execute simultaneously time-consuming algorithms across severals clustered computers.

Co-founders:
   Guillaume Abadie
   Thierry Cantenot

Yotta Distributed Computing Library's official code repository is available at <http://github.com/gabadie/yotta>.

## GNU GPL v3.0

Yotta is a free C library: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Daemon

```shell
git clone https://github.com/gabadie/yotta.git yotta --recursive
pip install -r yotta/python_requirements.txt
python yotta/src_daemon/daemon.py --port 5000
```

## Library "minimal" code

```c
#include <yotta.h>

/*
 * Slave invocation parameters
 */
typedef
struct slave_parameter_s
{
    yotta_addr_t prime_range_addr;
} slave_parameter_t;

/*
 * Natural integer range with a bitfield telling if each number is a prime
 */
typedef
struct prime_range_s
{
    // the offset of the range
    uint64_t offset;

    // bitfield saying if a given number is a prime or not
    uint8_t is_prime[512];
} prime_range_t;

/*
 * Slave invocation entry point
 */
void
main_slave(slave_parameter_t * parameters);

/*
 * Slave parrallel computation
 */
void
parrallel_compute(prime_range_s * prime_range);

/*
 * Main process entry point
 */
int
main(int argc, char const * const * argv)
{
    // yotta initialization
    yotta_init(argc, argv);

    // yotta context setup
    yotta_context_t context;
    yotta_context_init(&context, 8000);
    yotta_context_connect(&context, "127.0.0.1", 5000);
    /*
     * You might want to to connect to severals daemon
     */

    prime_range_t prime_range;
    prime_range.offset = 100000000000;

    slave_parameter_t slave_parameter;
    slave_parameter.prime_range_addr = yotta_addr(&prime_range);

    yotta_context_massive(&context,
        (yotta_massive_command_entry_t) main_slave,
        sizeof(slave_parameter), // parameter size
        &slave_parameter,        // parameter starting address
        0                        // parameter stride
    );

    // yotta context destruction
    yotta_context_destroy(&context);

    return 0;
}

void
main_slave(slave_parameter_t * slave_parameter)
{
    prime_range_t prime_range;

    // memory fetching from the master process
    {
        yotta_sync_t fetch_done;
        yotta_fetch_package(
            slave_parameter->prime_range_addr,
                                // the fetch destination address
            sizeof(uint64_t),   // the fetch size
            &prime_range,       // the fetch destination addres
            &fetch_done         // the fetch finish event
        );

        // waiting for the end of the fetch
        yotta_sync_wait(&fetch_done);
    }

    // dispatch threads
    yotta_dispatch((yotta_dispatch_func_t) parrallel_compute, &params, 0);

    // memory pushing to the master process
    {
        yotta_sync_t push_done;
        yotta_push_package(
            slave_parameter->prime_range_addr + sizeof(uint64_t),
                                            // the push destination address
            sizeof(prime_range.is_prime),   // the push size
            prime_range.is_prime,           // the push source address
            &push_done                      // the push finish event
        );

        /*
         * We have connected to only one daemon (127.0.0.1:5000), then it is just fine
         * to push entirely <prime_range.is_prime>. But with severals daemons, becareful
         * to not let a slave process write over other slave processes' pushes...
         */

        // waiting for the end of the push because prime_range is in the stack
        yotta_sync_wait(&push_done);
    }
}

void
parrallel_compute(prime_t * prime)
{
    uint64_t thread_id = 0;
    uint64_t thread_count = 1;

    // get current thread's unique id and the total thread count across all slaves
    yotta_get_global_id(&tid, &nb_threads);

    // [...] your parrallel algorithm right here
}
```

## Contribute to the project

### Workflow

1. Fork the official repository http://github.com/gabadie/yotta ;
2. Develop (and test) what you want;
3. Ask for a pull request.

### Command line tools:

```shell
git clone --recursive <your fork ssh address> yotta
cd yotta
pip install -r python_requirements.txt
```

* Build Yotta:
```
make
```

* Build Yotta debug:
```
make config=debug
```

* Build Yotta nightly (same as release, but with -g passed to GCC/Clang):
```
make config=nightly
```

* Test Yotta:
```
make test
```

* Pre-commit hook (testing debug and release configurations with GCC and Clang):
```
make hook
```

* More precisely...
```
make help
```

