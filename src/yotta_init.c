#include <string.h>
#include <stdlib.h>

#include "yotta_debug.h"
#include "yotta_init.h"
#include "yotta_slave.private.h"
#include "socket/yotta_socket_prefix.h"

char const *
yotta_executable_path = 0;


uint64_t
yotta_init(uint64_t argc, char const * const * argv)
{
    yotta_assert(argc >= 1);
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
            yotta_slave_parameters_t parameters;

            if (yotta_slave_parse_parameters(&parameters, argc - i - 1, argv + i + 1) != 0)
            {
                exit(yotta_process_failed_return);
            }

            if (yotta_slave_main(&parameters) != 0)
            {
                exit(yotta_process_failed_return);
            }

            exit(0);
        }
    }

    return 0;
}
