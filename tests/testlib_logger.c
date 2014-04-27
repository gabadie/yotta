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


#include <mk_test.h>
#include "../src/core/yotta_logger.h"
#include "../src/core/yotta_logger.private.h"

static yotta_log_msg_type_t g_msg_type;


void
test_slave_parse_parameters(void * user_data, yotta_logger_param_t const * param)
{
    test_assert(((uint64_t)g_msg_type) == 0xABC);
    test_assert(user_data == (void *)&g_msg_type);
    test_assert(param != 0);

    g_msg_type = param->type;
}


int
main()
{
    yotta_logger_error("this is an hidden error message");

    yotta_set_logger_entry(&test_slave_parse_parameters, &g_msg_type);

    g_msg_type = 0xABC;
    yotta_logger_error("this is an error message");
    test_assert(g_msg_type == YOTTA_LOG_ERROR);

    g_msg_type = 0xABC;
    yotta_logger_warning("this is a warning message");
    test_assert(g_msg_type == YOTTA_LOG_WARNING);

    return 0;
}
