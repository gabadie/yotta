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

#include <yotta.h>
#include <mk_test.h>
#include <inttypes.h>

#include "../src/core/yotta_addr_translation.private.h"


char
func_a()
{
    return 'a';
}

int
func_b(int m)
{
    return 1337 * m;
}

int
main(int argc, char const * const * argv)
{
    yotta_init(argc, argv);

    yotta_rel_addr_t rel_a = yotta_address_absolute_to_relative((uint64_t) &func_a);
    uint64_t abs_a = yotta_address_relative_to_absolute(rel_a);
    printf("func_a absolute address: %" PRIu64 "\n", abs_a);
    printf("func_a relative address: %" PRIu64 "\n", rel_a);

    test_assert2("Absolute address of a", abs_a == (uint64_t) &func_a);
    test_assert2("a call", ((char (*)()) abs_a)() == func_a());


    yotta_rel_addr_t rel_b = yotta_address_absolute_to_relative((uint64_t) &func_b);
    uint64_t abs_b = yotta_address_relative_to_absolute(rel_b);
    printf("func_b absolute address: %" PRIu64 "\n", abs_b);
    printf("func_b relative address: %" PRIu64 "\n", rel_b);

    test_assert2("Absolute address of b", abs_b == (uint64_t) &func_b);
    test_assert2("b call", ((int (*)(int)) abs_b)(42) == func_b(42));

    return 0;
}
