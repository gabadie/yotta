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
yotta_return_t
yotta_str_dec_to_ui64(uint64_t * out_number, char const * str);

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
