#ifndef _YOTTTA_STR_UTILS
#define _YOTTTA_STR_UTILS

#include "../yotta_prefix.h"
#include <stdio.h>


/*
 * @infos: gets the <file>'s cursor position
 *
 * @params <file>: the opened file descriptor
 *
 * @returns:
 *  - the <file> descriptor's position
 */
#define yotta_file_pos(file) \
    (uint64_t) ftell(file)

/*
 * @infos: gets the opened <file>'s size
 *
 * @params <file>: the opened file descriptor
 *
 * @returns:
 *  - the <file>'s size in bytes
 */
uint64_t
yotta_file_size(FILE * file);

#endif
