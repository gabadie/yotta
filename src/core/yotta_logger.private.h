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
#ifndef _YOTTAPRIVATE_LOGGER
#define _YOTTAPRIVATE_LOGGER


/*
 * @infos: append an error message to the log
 *
 * @param <msg>: error message to log
 */
void
yotta_logger_error(char const * msg);

/*
 * @infos: append an external error message to the log
 *
 * @param <msg>: error message to log
 * @param <errno>: the errno message to log
 */
void
yotta_logger_error_external(char const * msg, int errno);

/*
 * @infos: append a warning message to the log
 *
 * @param <msg>: warning message to log
 */
void
yotta_logger_warning(char const * msg);

/*
 * @infos: append a debug message to the log
 *
 * @param <msg>: debug message to log
 */
#ifdef YOTTA_DEBUG
void
yotta_logger_debug(char const * msg);
#else
#define yotta_logger_debug(msg)
#endif


#endif
