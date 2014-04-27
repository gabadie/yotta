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
#ifndef _YOTTTA_STR_UTILS
#define _YOTTTA_STR_UTILS

#include "../yotta_prefix.h"


/*
 * @infos: parse a uint64_t from a decimal string
 *
 * @param <out_number>: output number
 * @param <str>: string to parse from
 * @param <basis>: the explicit basis to convert from (0 if we want to guess it)
 *
 * @returns
 *  == YOTTA_SUCCESS if succeed
 *  == YOTTA_INVALID_VALUE if failed to parse <str>
 */
yotta_return_t
yotta_str_to_ui64(uint64_t * out_number, char const * str, uint64_t basis);

/*
 * @infos: generates a string represantation from a given <number> and <basis>
 *
 * @param <out_str>: output string
 * @param <number>: number to convert to
 * @param <basis>: number basis to convert to
 */
void
yotta_ui64_to_str(char * out_str, uint64_t number, uint64_t basis);


#endif
