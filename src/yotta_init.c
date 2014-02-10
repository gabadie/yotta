#include <string.h>
#include <stdlib.h>

#include "yotta_debug.h"
#include "yotta_init.h"
#include "yotta_init.private.h"
#include "yotta_address_translation.h"
#include "socket/yotta_socket_prefix.h"
#include "utils/yotta_str_utils.h"

char const *
yotta_executable_path = 0;


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
            yotta_cmd_parameters_t parameters;

            if (yotta_init_slave_parse(&parameters, argc - i - 1, argv + i + 1) != 0)
            {
                exit(-1);
            }



            exit(0);
        }
    }

    return 0;
}

#define yotta_init_slave_assert_arguments(argument_count) \
    { \
        if (argv + 1 == argv_end) \
        { \
            yotta_crash_msg("unexpected parameter end %s", *argv); \
            return -1; \
        } \
    }

#define yotta_init_slave_invalide_value() \
    { \
        yotta_crash_msg("invalid argument %s value: %s", argv[0], argv[1]); \
        return -1; \
    }

uint64_t
yotta_init_slave_parse(yotta_cmd_parameters_t * out_parameters, uint64_t argc, char const * const * argv)
{
    char const * const parameter_prefix = "--yotta-";
    size_t const parameter_prefix_length = strlen(parameter_prefix);

    yotta_assert(out_parameters != 0);
    yotta_assert(argc > 0);
    yotta_assert(argv != 0);

    char const * const * const argv_end = argv + argc;

    memset(out_parameters, 0, sizeof(yotta_cmd_parameters_t));

    while (argv != argv_end)
    {
        if (memcmp(*argv, parameter_prefix, parameter_prefix_length) != 0)
        {
            yotta_crash_msg("unknown parameter %s", *argv);
            return -1;
        }

        char const * const param_name = *argv + parameter_prefix_length;

        if (strcmp(param_name, "client-ip") == 0)
        {
            yotta_init_slave_assert_arguments(1);

            out_parameters->client_ip = argv[1];
            argv += 2;
        }
        else if (strcmp(param_name, "client-port") == 0)
        {
            uint64_t client_port;

            yotta_init_slave_assert_arguments(1);

            if (yotta_str_dec_to_ui64(&client_port, argv[1]) != 0 || client_port > 0xFFFF)
            {
                yotta_init_slave_invalide_value();
            }

            out_parameters->client_port = client_port;

            argv += 2;
        }
        else if (strcmp(param_name, "massive-function-offset") == 0)
        {
            yotta_init_slave_assert_arguments(1);

            if (yotta_str_dec_to_ui64(&out_parameters->massive_function_offset, argv[1]) != 0)
            {
                yotta_init_slave_invalide_value();
            }

            argv += 2;
        }
        else
        {
            yotta_crash_msg("unknown parameter %s", *argv);
            return -1;
        }
    }

    yotta_assert(argv <= argv_end);

    return 0;
}
