#include <string.h>
#include <stdlib.h>

#include "yotta_debug.h"
#include "yotta_slave.private.h"
#include "utils/yotta_str_utils.h"

#define yotta_slave_assert_arguments(argument_count) \
    { \
        if (argv + 1 > argv_end) \
        { \
            yotta_crash_msg("unexpected parameter end %s", *argv); \
            return -1; \
        } \
    }

#define yotta_slave_invalide_value() \
    { \
        yotta_crash_msg("invalid argument %s value: %s", argv[0], argv[1]); \
        return -1; \
    }

uint64_t
yotta_slave_parse_parameters(yotta_slave_parameters_t * out_parameters, uint64_t argc, char const * const * argv)
{
    char const * const parameter_prefix = "--yotta-";
    size_t const parameter_prefix_length = strlen(parameter_prefix);

    yotta_assert(out_parameters != 0);
    yotta_assert(argc > 0);
    yotta_assert(argv != 0);

    char const * const * const argv_end = argv + argc;

    memset(out_parameters, 0, sizeof(yotta_slave_parameters_t));

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
            yotta_slave_assert_arguments(1);

            out_parameters->client_ip = argv[1];
            argv += 2;
        }
        else if (strcmp(param_name, "client-port") == 0)
        {
            uint64_t client_port;

            yotta_slave_assert_arguments(1);

            if (yotta_str_dec_to_ui64(&client_port, argv[1]) != 0 || client_port > 0xFFFF)
            {
                yotta_slave_invalide_value();
            }

            out_parameters->client_port = client_port;

            argv += 2;
        }
        else if (strcmp(param_name, "massive-function-offset") == 0)
        {
            yotta_slave_assert_arguments(1);

            if (yotta_str_dec_to_ui64(&out_parameters->massive_function_offset, argv[1]) != 0)
            {
                yotta_slave_invalide_value();
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

uint64_t
yotta_slave_main(yotta_slave_parameters_t const * out_parameters)
{
    if (out_parameters->client_ip == 0)
    {
        yotta_crash_msg("missing client ip");
        return -1;
    }

    return 0;
}
