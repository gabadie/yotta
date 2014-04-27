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

yotta_return_t
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
            return YOTTA_INVALID_ENUM;
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

            if (yotta_str_to_ui64(&client_port, argv[1], 0) != 0 || client_port > 0xFFFF || client_port == 0)
            {
                yotta_slave_invalide_value();
            }

            out_parameters->client_port = client_port;

            argv += 2;
        }
        else
        {
            return YOTTA_INVALID_ENUM;
        }
    }

    yotta_assert(argv <= argv_end);

    return YOTTA_SUCCESS;
}
