

#include <mk_test.h>
#include <string.h>
#include "../src/yotta_slave.private.h"

void
test_slave_parse_parameters()
{
    yotta_cmd_parameters_t parameters;

    {
        char const * const argv[] = {
            "--yotta-client-ip",
            "127.0.0.1"
        };

        test_assert(yotta_slave_parse_parameters(&parameters, 2, argv) == 0);
        test_assert(strcmp(parameters.client_ip, "127.0.0.1") == 0);

        test_assert(yotta_slave_parse_parameters(&parameters, 1, argv) == -1ull);
    }

    {
        char const * const argv[] = {
            "--yotta-client-port",
            "5000"
        };

        test_assert(yotta_slave_parse_parameters(&parameters, 2, argv) == 0);
        test_assert(parameters.client_port == 5000);

        test_assert(yotta_slave_parse_parameters(&parameters, 1, argv) == -1ull);
    }

    {
        char const * const argv[] = {
            "--yotta-client-port",
            "70000"
        };

        test_assert(yotta_slave_parse_parameters(&parameters, 2, argv) == -1ull);
    }

    {
        char const * const argv[] = {
            "--yotta-massive-function-offset",
            "8000"
        };

        test_assert(yotta_slave_parse_parameters(&parameters, 2, argv) == 0);
        test_assert(parameters.massive_function_offset == 8000);
    }

    {
        char const * const argv[] = {
            "--yotta-hello",
            "8000"
        };

        test_assert(yotta_slave_parse_parameters(&parameters, 2, argv) == -1ull);
    }

    {
        char const * const argv[] = {
            "--hello",
            "8000"
        };

        test_assert(yotta_slave_parse_parameters(&parameters, 2, argv) == -1ull);
    }
}

int
main()
{
    test_slave_parse_parameters();

    return 0;
}
