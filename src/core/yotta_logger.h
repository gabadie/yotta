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
    YOTTA_LOG_ERROR             = 0x0,
    YOTTA_LOG_EXTERNAL_ERROR    = 0x1,
    YOTTA_LOG_WARNING           = 0x2,
    YOTTA_LOG_DEBUG             = 0x3
}
yotta_log_msg_type_t;

/*
 * @infos: logger informations passed to the logger entry
 */
typedef struct
yotta_logger_param_s
{
    // the type
    yotta_log_msg_type_t type;

    // the message
    char const * msg;

    // the thread id
    uint64_t thread_id;

    // the process id
    uint64_t process_id;

    // the local id
    uint64_t local_id;

    // the local size
    uint64_t local_count;

    // the group id
    uint64_t group_id;

    // the group count
    uint64_t group_count;

    // the global id
    uint64_t global_id;

    // the global count
    uint64_t global_count;
}
yotta_logger_param_t;

/*
 * @infos: defines yotta logger entry function pointer type
 *
 * The logger entry received 3 parameters:
 *  - void * user_data: specific user data
 *  - yotta_logger_param_t const * infos: the message
 */
typedef void (* yotta_logger_entry_t)(void *, yotta_logger_param_t const *);

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
