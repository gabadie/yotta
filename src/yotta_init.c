#include <string.h>
#include <stdlib.h>

#include "yotta_debug.h"
#include "yotta_init.h"
#include "yotta_address_translation.h"
#include "socket/yotta_socket_prefix.h"

char const *
yotta_executable_path = 0;


static uint64_t
yotta_init_slave(uint64_t argc, char const * const * argv);


uint64_t
yotta_init(uint64_t argc, char const * const * argv)
{
    yotta_assert(argc > 1);
    yotta_assert(argv != 0);

    yotta_executable_path = argv[0];

    if (yotta_init_csocket())
    {
        return -1;
    }

    for (uint64_t i = 1; i < argc; i++)
    {
        yotta_assert(argv[i] != 0);

        if (strcmp(argv[i], "--yotta") == 0)
        {
            uint64_t returned = yotta_init_slave(argc - i - 1, argv + i + 1);

            exit((int)returned);
        }
    }

    return 0;
}

uint64_t
yotta_init_slave(uint64_t argc __attribute__((unused)), char const * const * argv __attribute__((unused)))
{

    return 0;
}
