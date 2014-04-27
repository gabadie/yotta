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
#include "../src/utils/yotta_math.h"


static
void
test_is_pow2()
{
    test_assert(yotta_math_is_pow2(0) == 1); // limit case

    test_assert(yotta_math_is_pow2(1) == 1);
    test_assert(yotta_math_is_pow2(2) == 1);
    test_assert(yotta_math_is_pow2(3) == 0);
    test_assert(yotta_math_is_pow2(4) == 1);
    test_assert(yotta_math_is_pow2(1024) == 1);
    test_assert(yotta_math_is_pow2(~0u) == 0);
    test_assert(yotta_math_is_pow2(~0ull) == 0);
}

static
void
test_ceil_pow2_ui64()
{
    test_assert(yotta_math_ceil_pow2_ui64(0) == 0); // limit case
    test_assert(yotta_math_ceil_pow2_ui64(1) == 1);
    test_assert(yotta_math_ceil_pow2_ui64(2) == 2);
    test_assert(yotta_math_ceil_pow2_ui64(3) == 4);
    test_assert(yotta_math_ceil_pow2_ui64(24) == 32);
    test_assert(yotta_math_ceil_pow2_ui64(~0ull) == 0); // limit case
}

int
main()
{
    test_is_pow2();
    test_ceil_pow2_ui64();
    return 0;
}

