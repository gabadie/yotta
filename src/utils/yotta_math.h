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
#ifndef _YOTTA_MATH
#define _YOTTA_MATH

#include "../yotta_prefix.h"


/*
 * @infos: Checks if a power of two
 *
 * @param <x> is the number to check
 *
 * @caution 0 will succeed the condition
 */
#define yotta_math_is_pow2(x) \
    ((((x) - 1) & (x)) == 0)


/*
 * @infos: Rounds <x> upward to a power of two
 *
 * @param <x>: is the value to round up to
 *
 * @returns: The power of two
 *
 * @caution x=0 will returns 0
 */
uint64_t
yotta_math_ceil_pow2_ui64(uint64_t x);

uint32_t
yotta_math_ceil_pow2_ui32(uint32_t x);


#endif
