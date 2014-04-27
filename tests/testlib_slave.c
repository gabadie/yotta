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


#include <string.h>
#include "testhelper_init.h"
#include "../src/slave/yotta_slave.private.h"

void
test_slave_parse_parameters()
{
    yotta_slave_parameters_t parameters;

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
    testhelper_init();

    test_slave_parse_parameters();

    return 0;
}
