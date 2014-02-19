#ifndef _YOTTA_RETURN
#define _YOTTA_RETURN

#include "yotta_prefix.h"


/*
 * @infos: returned when a yotta function has successed
 */
#define YOTTA_SUCCESS 0

/*
 * @infos: returned if a yotta function has received an invalid enumeration parameter
 */
#define YOTTA_INVALID_ENUM ((uint64_t) -1)

/*
 * @infos: returned if a yotta function has received an invalid value parameter
 */
#define YOTTA_INVALID_VALUE ((uint64_t) -2)

/*
 * @infos: returned if a yotta function can't processed a invalid operation
 */
#define YOTTA_INVALID_OPERATION ((uint64_t) -3)

/*
 * @infos: returned if a yotta function has an unexpected fail
 */
#define YOTTA_UNEXPECTED_FAIL ((uint64_t) -4)

#endif
