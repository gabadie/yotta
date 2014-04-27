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

#include "../src/utils/yotta_str_utils.h"
#include <mk_test.h>
#include <string.h>


static
void
test_str_to_ui64_null()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 2) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 8) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 10) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0", 16) == 0 && output == 0);
}

static
void
test_str_to_ui64_dec()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "0a", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "01", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "hello", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "+1", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "-1", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "0x1", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "18446744073709551616", 10) != 0);

    test_assert(yotta_str_to_ui64(&output, "0", 10) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "00", 10) != 0);
    test_assert(yotta_str_to_ui64(&output, "1", 10) == 0 && output == 1);
    test_assert(yotta_str_to_ui64(&output, "130", 10) == 0 && output == 130);
    test_assert(
        yotta_str_to_ui64(&output, "18446744073709551615", 10) == 0 &&
        output == 18446744073709551615ull
    );
}

static
void
test_str_to_ui64_hex()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "g", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "0g", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "01", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "hello", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "+1", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "-1", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "0x1", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "10000000000000000", 16) != 0);

    test_assert(yotta_str_to_ui64(&output, "0", 16) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "00", 16) != 0);
    test_assert(yotta_str_to_ui64(&output, "1", 16) == 0 && output == 1);
    test_assert(yotta_str_to_ui64(&output, "fF", 16) == 0 && output == 0xfF);
    test_assert(yotta_str_to_ui64(&output, "1abcdef", 16) == 0 && output == 0x1abcdef);
    test_assert(yotta_str_to_ui64(&output, "130", 16) == 0 && output == 0x130);
    test_assert(
        yotta_str_to_ui64(&output, "FFFFFFFFFFFFFFFF", 16) == 0 &&
        output == 0xFFFFFFFFFFFFFFFFull
    );
}

static
void
test_str_to_ui64_detect()
{
    uint64_t output = 0;

    test_assert(yotta_str_to_ui64(&output, "0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0b0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "00", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0x0", 0) == 0 && output == 0);

    test_assert(yotta_str_to_ui64(&output, "0", 0) == 0 && output == 0);
    test_assert(yotta_str_to_ui64(&output, "0b10", 0) == 0 && output == 0x2);
    test_assert(yotta_str_to_ui64(&output, "042", 0) == 0 && output == 042);
    test_assert(yotta_str_to_ui64(&output, "42", 0) == 0 && output == 42);
    test_assert(yotta_str_to_ui64(&output, "0x42", 0) == 0 && output == 0x42);
}

static
void
testhelper_ui64_to_str(char const * string, uint64_t number, uint64_t basis)
{
    char result[32];

    yotta_ui64_to_str(result, number, basis);

    test_assert(strcmp(string, result) == 0);
}

static
void
test_ui64_to_str()
{
    testhelper_ui64_to_str("0", 0, 2);
    testhelper_ui64_to_str("0", 0, 8);
    testhelper_ui64_to_str("0", 0, 10);
    testhelper_ui64_to_str("0", 0, 16);

    testhelper_ui64_to_str("1", 0x1, 2);
    testhelper_ui64_to_str("10", 0x2, 2);

    testhelper_ui64_to_str("7", 7, 10);
    testhelper_ui64_to_str("10", 10, 10);
    testhelper_ui64_to_str("436", 436, 10);

    testhelper_ui64_to_str("7", 07, 8);
    testhelper_ui64_to_str("10", 010, 8);
    testhelper_ui64_to_str("436", 0436, 8);

    testhelper_ui64_to_str("7", 0x7, 16);
    testhelper_ui64_to_str("10", 0x10, 16);
    testhelper_ui64_to_str("436", 0x436, 16);
}

int
main()
{
    test_str_to_ui64_null();
    test_str_to_ui64_dec();
    test_str_to_ui64_hex();
    test_str_to_ui64_detect();
    test_ui64_to_str();
    return 0;
}

