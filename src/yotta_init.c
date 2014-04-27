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

#include "yotta_init.h"
#include "core/yotta_return.private.h"
#include "slave/yotta_slave_context.private.h"
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

        if (yotta_slave_context_main(&parameters) != 0)
        {
            yotta_logger_error("yotta_slave_context_main failed");

            exit(yotta_process_failed_return);
        }

        exit(0);
    }

    return YOTTA_SUCCESS;
}
