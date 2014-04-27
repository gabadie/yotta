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
#ifndef _YOTTA_MEMORY
#define _YOTTA_MEMORY

#include "../yotta_prefix.h"


/*
 * Defines the yotta's default memory alignement
 */
#define YOTTA_DEFAULT_ALIGNMENT 16

/*
 * Compilor specific binding for the alignof keyword
 */
#define _yotta_alignof(type) \
    __alignof(type)

/*
 * Gets the yotta alignment for a given type
 */
#define yotta_alignof(type) \
    ( \
        (_yotta_alignof(type) < (YOTTA_DEFAULT_ALIGNMENT)) \
            ? (YOTTA_DEFAULT_ALIGNMENT) \
            : _yotta_alignof(type) \
    )

/*
 * Defines the yotta_align() keyword
 */
#define yotta_align(align) __attribute__((aligned(align)))

/*
 * Defines the yotta_default_align keyword
 */
#define yotta_default_align yotta_align(YOTTA_DEFAULT_ALIGNMENT)

/*
 * @infso: forward declaration of yotta_memory_prefix_t
 */
typedef struct
yotta_memory_prefix_s
yotta_memory_prefix_t;

/*
 * @infos: defines yotta memory allocator function pointer type
 *
 * The tcp command entry received 2 parameters:
 *  - void * user_data: the user data
 *  - size_t size: the size to allocates
 *  - size_t alignment: the alignment to allocates
 *
 * returns:
 *  properly initialized yotta_memory_prefix_t directly followed by the memory
 *  block itself.
 */
typedef yotta_memory_prefix_t * (*yotta_memory_alloc_func_t)(void *, size_t, size_t);

/*
 * @infos: defines yotta memory free function pointer type
 *
 * The tcp command entry received 2 parameters:
 *  - yotta_memory_prefix_t * memory_prefix: the memory prefix to free
 */
typedef void (*yotta_memory_free_func_t)(yotta_memory_prefix_t *);

/*
 * @infso: defines the memory block prefix
 */
struct
yotta_memory_prefix_s
{
    /*
     * The function pointer that will free this memory prefix
     */
    yotta_memory_free_func_t free_function;

    /*
     * Some user data that the allocator might want to leave to the free function
     */
    void * user_data;

    /*
     * The memory block size
     */
    size_t size;

    /*
     * The number of coins for the retain/release mechanism
     */
    size_t coins;
}
yotta_default_align;

/*
 * @infos: gets memory prefix's address
 *
 * @param <ptr>: the pointer to query
 */
#define yotta_memory_prefix(ptr) \
    (((yotta_memory_prefix_t *) (ptr)) - 1)

/*
 * @infos: gets memory size
 *
 * @param <ptr>: the pointer to query
 */
#define yotta_memory_size(ptr) \
    yotta_memory_prefix(ptr)->size

/*
 * @infos: allocate a memory block from its given size
 *
 * @param <size>: the memory size to allocate
 * @param <alignment>: the memory alignment
 *
 * @returns: pointer on the allocated memory
 */
void * yotta_default_align
yotta_alloc(size_t size, size_t alignment);

/*
 * @infos: allocate a memory block from its given size with the default alignment
 *
 * @param <size>: the memory block's size
 *
 * @returns: pointer on the allocated memory
 */
#define yotta_alloc_d(size) \
    yotta_alloc(size, YOTTA_DEFAULT_ALIGNMENT)

/*
 * @infos: allocate a struct from its given name
 *
 * @param <struct_name>: the struct name
 *
 * @returns: (struct_name *) pointer on the allocated memory
 */
#define yotta_alloc_s(struct_name) \
    (struct_name *) yotta_alloc(sizeof(struct_name), yotta_alignof(struct_name))

/*
 * @infos: allocate a struct array from its given name
 *
 * @param <struct_name>: the struct name
 * @param <element_count>: the array size
 *
 * @returns: (struct_name *) pointer on the allocated memory array
 */
#define yotta_alloc_sa(struct_name,element_count) \
    (struct_name *) yotta_alloc(sizeof(struct_name) * (element_count), yotta_alignof(struct_name))

/*
 * @infos: frees memory
 *
 * @param <ptr>: memory address to free
 */
void
yotta_free(void * ptr);

/*
 * @infos: sets the global memory allocator
 *
 * @param <allocator>: the allocator entry point
 * @param <user_data>: the custom user data
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <allocator> is null
 */
yotta_return_t
yotta_memory_set_allocator(yotta_memory_alloc_func_t allocator, void * user_data);


#endif
