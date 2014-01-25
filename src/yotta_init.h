#ifndef _YOTTA_INIT
#define _YOTTA_INIT

#include "yotta_prefix.h"

/*
 * @infos: init the yotta library
 *
 * @param <argc>: the main's argc parameter
 * @param <argv>: the main's argv parameter
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
uint64_t
yotta_init(uint64_t argc, char ** argv);

#endif
