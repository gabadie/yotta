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
#ifndef _YOTTA_ATOMIC
#define _YOTTA_ATOMIC

#include "../yotta_prefix.h"


#if defined(__GNUC__)


/*
 * @infos: defines atomic operations
 *
 * @doc: http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html
 */

#define yotta_atomic_fetch_add(ptr, value) \
    __sync_fetch_and_add(ptr, value)

#define yotta_atomic_fetch_sub(ptr, value) \
    __sync_fetch_and_sub(ptr, value)

#define yotta_atomic_fetch_or(ptr, value) \
    __sync_fetch_and_or(ptr, value)

#define yotta_atomic_fetch_and(ptr, value) \
    __sync_fetch_and_and(ptr, value)

#define yotta_atomic_fetch_xor(ptr, value) \
    __sync_fetch_and_xor(ptr, value)

#define yotta_atomic_fetch_nand(ptr, value) \
    __sync_fetch_and_nand(ptr, value)


#define yotta_atomic_add_fetch(ptr, value) \
    __sync_add_and_fetch(ptr, value)

#define yotta_atomic_sub_fetch(ptr, value) \
    __sync_sub_and_fetch(ptr, value)

#define yotta_atomic_or_fetch(ptr, value) \
    __sync_or_and_fetch(ptr, value)

#define yotta_atomic_and_fetch(ptr, value) \
    __sync_and_and_fetch(ptr, value)

#define yotta_atomic_xor_fetch(ptr, value) \
    __sync_xor_and_fetch(ptr, value)

#define yotta_atomic_nand_fetch(ptr, value) \
    __sync_nand_and_fetch(ptr, value)


#define yotta_atomic_compare_swap_b(ptr, oldval, newval) \
    __sync_bool_compare_and_swap(ptr, oldval, newval)

#define yotta_atomic_compare_swap(ptr, oldval, newval) \
    __sync_val_compare_and_swap(ptr, oldval, newval)

#else
#error "yotta_atomic.h not supported"

#endif


#endif //_YOTTA_ATOMIC
