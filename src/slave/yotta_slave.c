
#include <string.h>
#include <stdlib.h>

#include "yotta_slave.private.h"
#include "../core/yotta_debug.h"
#include "../utils/yotta_str_utils.h"

#define yotta_slave_assert_arguments(argument_count) \
    { \
        if (argv + argument_count >= argv_end) \
        { \
            return -1; \
        } \
    }

#define yotta_slave_invalide_value() \
    { \
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

    while (argv < argv_end)
    {
        if (memcmp(*argv, parameter_prefix, parameter_prefix_length) != 0)
        {
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

            if (yotta_str_dec_to_ui64(&client_port, argv[1]) != 0 || client_port > 0xFFFF || client_port == 0)
            {
                yotta_slave_invalide_value();
            }

            out_parameters->client_port = client_port;

            argv += 2;
        }
        else
        {
            return -1;
        }
    }

    yotta_assert(argv <= argv_end);

    return 0;
}

uint64_t
yotta_slave_main(yotta_slave_parameters_t const * out_parameters)
{
    yotta_assert(out_parameters != 0);
    yotta_assert(out_parameters->client_ip != 0);
    yotta_assert(out_parameters->client_port != 0);

    (void) out_parameters;

    return 0;
}
