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
#ifndef _YOTTA_ADDR_TRANSLATION
#define _YOTTA_ADDR_TRANSLATION

#include "../yotta_prefix.h"


/*
 * @infos: relative source code address
 */
typedef uint64_t yotta_rel_addr_t;


/*
 * @infos: convert an absolute address to a relative one
 *
 * @param <address>: absolute address to convert
 *
 * @returns:
 *  The relative address corresponding to the given absolute address
 */
yotta_rel_addr_t
yotta_address_absolute_to_relative(uint64_t address);

/*
 * @infos: convert an (function) offset to its absolute address
 *
 * @param <offset>: offset to convert
 *
 * @returns:
 *  The absolute address corresponding to the given offset
 */
uint64_t
yotta_address_relative_to_absolute(yotta_rel_addr_t offset);

#endif //_YOTTA_ADDRESS_TRANSLATION
