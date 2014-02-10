#ifndef _YOTTA_ADDRESS_TRANSLATION
#define _YOTTA_ADDRESS_TRANSLATION

#include "yotta_prefix.h"


/*
 * @infos: convert an absolute address to a relative one
 *
 * @param <address>: absolute address to convert
 *
 * @returns:
 *  The relative address corresponding to the given absolute address
 */
uint64_t 
yotta_address_absolute_to_relative(uint64_t address);

/*
 * @infos: convert an (function) offset to its absolute address
 *
 * @param <offset>: offset to convert
 *
 * @returns:
 *  The absolute address corresponding to the given offset
 */
uint64_t
yotta_address_relative_to_absolute(uint64_t offset);

#endif //_YOTTA_ADDRESS_TRANSLATION
