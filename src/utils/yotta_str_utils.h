#ifndef _YOTTTA_STR_UTILS
#define _YOTTTA_STR_UTILS

#include "../yotta_prefix.h"


/*
 * @infos: parse a uint64_t from a decimal string
 *
 * @param <out_number>: output number
 * @param <str>: string to parse from
 * @param <basis>: the explicit basis to convert from (0 if we want to guess it)
 *
 * @returns
 *  == YOTTA_SUCCESS if succeed
 *  == YOTTA_INVALID_VALUE if failed to parse <str>
 */
yotta_return_t
yotta_str_to_ui64(uint64_t * out_number, char const * str, uint64_t basis);

/*
 * @infos: generates a string represantation from a given <number> and <basis>
 *
 * @param <out_str>: output string
 * @param <number>: number to convert to
 * @param <basis>: number basis to convert to
 */
void
yotta_ui64_to_str(char * out_str, uint64_t number, uint64_t basis);


#endif
