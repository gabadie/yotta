#ifndef _YOTTA_LOGGER
#define _YOTTA_LOGGER

#include "../yotta_prefix.h"


/*
 * @infos: defines the yotta log message type
 *
 * @note: YOTTA_LOG_DEBUG message will only be triggered if yotta has been compiled with YOTTA_DEBUG
 */
typedef enum
{
    YOTTA_LOG_ERROR     = 0x0,
    YOTTA_LOG_WARNING   = 0x1,
    YOTTA_LOG_DEBUG     = 0x2
}
yotta_log_msg_type_t;

/*
 * @infos: defines yotta logger entry function pointer type
 *
 * The logger entry received 3 parameters:
 *  - yotta_log_msg_type_t msg_type: specifying the message's type
 *  - char const * msg: the message
 *  - void * user_data: specific user data
 */
typedef void (* yotta_logger_entry_t)(yotta_log_msg_type_t, char const *, void *);

/*
 * @infos: sets the yotta's main logger entry point.
 *
 * @param <callback>: logger callback
 * @param <user_data>: specific user data
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
yotta_return_t
yotta_set_logger_entry(yotta_logger_entry_t callback, void * user_data);

#endif
