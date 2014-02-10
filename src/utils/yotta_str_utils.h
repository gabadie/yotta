#ifndef _YOTTTA_STR_UTILS
#define _YOTTTA_STR_UTILS

#include "../yotta_prefix.h"


/*
 * @infos: parse a uint64_t from a decimal string
 *
 * @param <out_number>: output number
 * @param <str>: string to parse from
 *
 * @returns
 *  == <0> if succeed
 *  != <0> elsewhere
 */
uint64_t
yotta_str_dec_to_ui64(uint64_t * out_number, char const * str);

#endif
