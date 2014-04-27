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
#ifndef _YOTTA_LOGGER
#define _YOTTA_LOGGER

#include "../yotta_prefix.h"


/*
 * @infos: defines the yotta log message type
 *
 * @note: YOTTA_LOG_DEBUG message will only be triggered if yotta has been compiled with YOTTA_DEBUG
 */
typedef enum
{
    YOTTA_LOG_ERROR             = 0x0,
    YOTTA_LOG_EXTERNAL_ERROR    = 0x1,
    YOTTA_LOG_WARNING           = 0x2,
    YOTTA_LOG_DEBUG             = 0x3
}
yotta_log_msg_type_t;

/*
 * @infos: logger informations passed to the logger entry
 */
typedef struct
yotta_logger_param_s
{
    // the type
    yotta_log_msg_type_t type;

    // the message
    char const * msg;

    // the thread id
    uint64_t thread_id;

    // the process id
    uint64_t process_id;

    // the local id
    uint64_t local_id;

    // the local size
    uint64_t local_count;

    // the group id
    uint64_t group_id;

    // the group count
    uint64_t group_count;

    // the global id
    uint64_t global_id;

    // the global count
    uint64_t global_count;
}
yotta_logger_param_t;

/*
 * @infos: defines yotta logger entry function pointer type
 *
 * The logger entry received 3 parameters:
 *  - void * user_data: specific user data
 *  - yotta_logger_param_t const * infos: the message
 */
typedef void (* yotta_logger_entry_t)(void *, yotta_logger_param_t const *);

/*
 * @infos: sets the yotta's main logger entry point.
 *
 * @param <callback>: logger callback
 * @param <user_data>: specific user data
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
yotta_return_t
yotta_set_logger_entry(yotta_logger_entry_t callback, void * user_data);

#endif
