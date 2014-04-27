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
#ifndef _YOTTAPRIVATE_RETURN
#define _YOTTAPRIVATE_RETURN

#include "yotta_return.h"
#include "yotta_debug.h"
#include "yotta_logger.private.h"


/*
 * @infos: returns YOTTA_INVALID_VALUE
 *
 * @param <function_name>: the function's name
 * @param <arg_name>: the argument's name
 */
#define yotta_return_inv_value(function_name,arg_name) \
    {\
        yotta_logger_error( \
            "INVALID_VALUE: <" \
            yotta_to_str(arg_name) \
            "> of " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_INVALID_VALUE; \
    }

/*
 * @infos: returns YOTTA_INVALID_ENUM
 *
 * @param <function_name>: the function's name
 * @param <arg_name>: the argument's name
 */
#define yotta_return_inv_enum(function_name,arg_name) \
    {\
        yotta_logger_error( \
            "INVALID_ENUMERATION: <" \
            yotta_to_str(arg_name) \
            "> of " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_INVALID_ENUM; \
    }

/*
 * @infos: returns YOTTA_INVALID_OPERATION
 *
 * @param <function_name>: the function's name
 */
#define yotta_return_inv_op(function_name) \
    {\
        yotta_logger_error( \
            "INVALID_OPERATION: " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_INVALID_OPERATION; \
    }

/*
 * @infos: returns YOTTA_UNEXPECTED_FAIL
 *
 * @param <function_name>: the function's name
 */
#define yotta_return_unexpect_fail(function_name) \
    {\
        yotta_logger_error( \
            "UNEXPECTED_FAIL: " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_UNEXPECTED_FAIL; \
    }

#endif
