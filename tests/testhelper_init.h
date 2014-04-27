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
#ifndef _TESTHELPER_INIT
#define _TESTHELPER_INIT

#include <stdio.h>
#include <yotta.h>
#include <mk_test.h>


/*
 * @infos: tests' logger entry
 *
 * @params: See yotta_logger_entry_t in yotta_logger.h
 */
static
void
testhelper_log(void * user_data, yotta_logger_param_t const * param)
{
    static char const * const msg_type_name[] = {
        "ERROR",
        "WARNING",
        "DEBUG"
    };

    test_assert(user_data == 0);
    test_assert(param != 0);
    test_assert(param->type < (sizeof(msg_type_name) / sizeof(typeof(msg_type_name))));

    printf("%s: %s\n", msg_type_name[param->type], param->msg);
}

/*
 * @infos: sets up the yotta test environment
 */
static
void
testhelper_init()
{
    char const * const exec_name = __FILE__;

    yotta_set_logger_entry(&testhelper_log, 0);

    test_assert(yotta_init(1, &exec_name) == 0);
}


#endif
