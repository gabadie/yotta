#ifndef _YOTTA_ADDR
#define _YOTTA_ADDR

#include "../yotta_prefix.h"


/*
 * @infos: multiplatform address
 */
typedef uint64_t yotta_addr_t;


/*
 * @infos: gets the multiplatform address
 *
 * @param <address>: the pointer to address to
 *
 * @returns:
 *  The multiplatform address corresponding to the given pointer
 */
#define yotta_addr(addr) \
    ((yotta_addr_t) (addr))

#endif //_YOTTA_ADDR
