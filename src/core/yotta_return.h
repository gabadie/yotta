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
#ifndef _YOTTA_RETURN
#define _YOTTA_RETURN

#include "../yotta_prefix.h"


/*
 * Defines the yotta return type
 */
typedef uint64_t yotta_return_t;

/*
 * @infos: returned when a yotta function has successed
 */
#define YOTTA_SUCCESS 0

/*
 * @infos: returned if a yotta function has received an invalid enumeration parameter
 */
#define YOTTA_INVALID_ENUM ((uint64_t) -1)

/*
 * @infos: returned if a yotta function has received an invalid value parameter
 */
#define YOTTA_INVALID_VALUE ((uint64_t) -2)

/*
 * @infos: returned if a yotta function can't processed a invalid operation
 */
#define YOTTA_INVALID_OPERATION ((uint64_t) -3)

/*
 * @infos: returned if a yotta function has an unexpected fail
 */
#define YOTTA_UNEXPECTED_FAIL ((uint64_t) -4)

#endif
