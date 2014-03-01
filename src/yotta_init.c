#include <string.h>
#include <stdlib.h>

#include "yotta_init.h"
#include "core/yotta_return.private.h"
#include "slave/yotta_slave.private.h"
#include "socket/yotta_socket_prefix.h"

char const *
yotta_executable_path = 0;


uint64_t
yotta_init(uint64_t argc, char const * const * argv)
{
    if (argc == 0)
    {
        yotta_return_inv_value(yotta_init, argc);
    }
    else if (argv == 0)
    {
        yotta_return_inv_value(yotta_init, argv);
    }

    yotta_executable_path = argv[0];

    if (yotta_init_csocket())
    {
        yotta_return_unexpect_fail(yotta_init);
    }

    for (uint64_t i = 1; i < argc; i++)
    {
        if (argv[i] == 0)
        {
            yotta_return_inv_value(yotta_init, argv);
        }

        if (strcmp(argv[i], "--yotta") != 0)
        {
            continue;
        }

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

    return YOTTA_SUCCESS;
}
