#ifndef _YOTTA_INIT
#define _YOTTA_INIT

#include "yotta_prefix.h"

/*
 * @infos: return value when Yotta failed to parses parameters
 */
static int const
yotta_process_failed_return = 2;

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
yotta_init(uint64_t argc, char const * const * argv);

/*
 * @infos: contains executable's path (argv[0])
 */
extern char const *
yotta_executable_path;


#endif
