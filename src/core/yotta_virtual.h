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
#ifndef _YOTTA_VIRTUAL
#define _YOTTA_VIRTUAL

#include "yotta_struct.h"


/*
 * Gets an object's virtual table
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table to cast to
 */
#define yotta_vtable(this,vtable_type) \
    (*(vtable_type const **) (this))

/*
 * Sets an object's virtual table
 *
 * @param <this> is the pointer on the object
 * @param <vtable> is the vtable to set
 */
#define yotta_vtable_set(this,vtable) \
    ((*(void const **) (this)) = (void const *)(vtable))

/*
 * Gets an object's virtual function
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table
 * @param <function_name> the name of the virtual function
 */
#define yotta_vfunc(this,vtable_type,function_name) \
    (yotta_vtable(this, vtable_type)->function_name)

/*
 * Calls an object's virtual function
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table
 * @param <function_name> the name of the virtual function
 * @param <...> the virtual function parameters
 *
 * @example:
 *  yotta_vcall(my_object, my_vtable_t, my_function_name, foo, bar)
 */
#define yotta_vcall(this,vtable_type,function_name) \
    (yotta_vfunc(this, vtable_type, function_name)(this))

#define yotta_vcall_params(this,vtable_type,function_name,...) \
    (yotta_vfunc(this, vtable_type, function_name)(this, __VA_ARGS__))

/*
 * Calls an object's virtual release function
 *
 * @param <this> is the pointer on the object to release
 */
#define yotta_vcall_release(this) \
    ((**((void (***)(typeof(this))) (this)))(this))


#endif
