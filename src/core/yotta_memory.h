#ifndef _YOTTA_MEMORY
#define _YOTTA_MEMORY

#include <stdlib.h>


/*
 * @infos: allocate a struct from its given name
 *
 * @param <struct_name>: the struct name
 *
 * @returns: (struct_name *) pointer on the allocated memory
 */
#define yotta_alloc_s(struct_name) \
    (struct_name *) malloc(sizeof(struct_name))

/*
 * @infos: frees memory
 *
 * @param <ptr>: memory address to free
 */
#define yotta_free(ptr) \
    free((void *) ptr)


#endif
