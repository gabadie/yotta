
#include "yotta_logger.h"
#include "yotta_logger.private.h"


/*
 * @infos: defines yotta logger's global variables
 */
typedef struct
yotta_global_logger_s
{
    yotta_logger_entry_t callback;
    void * user_data;
}
yotta_global_logger_t;

// yotta logger globals
static yotta_global_logger_t
yotta_global_logger;


/*
 * @infos: log out a message
 *
 * @param <msg_type>: message type
 * @param <msg>: message content
 *
 * @returns: void
 */
#define yotta_logger(msg_type,msg) \
    if (yotta_global_logger.callback) \
    { \
        yotta_global_logger.callback(msg_type, msg, yotta_global_logger.user_data); \
    }


uint64_t
yotta_set_logger_entry(yotta_logger_entry_t callback, void * user_data)
{
    yotta_global_logger.callback = callback;
    yotta_global_logger.user_data = user_data;

    return 0;
}

void
yotta_logger_error(char const * msg)
{
    yotta_logger(YOTTA_LOG_ERROR, msg);
}

void
yotta_logger_warning(char const * msg)
{
    yotta_logger(YOTTA_LOG_WARNING, msg);
}

#ifdef YOTTA_DEBUG
void
yotta_logger_debug(char const * msg)
{
    yotta_logger(YOTTA_LOG_DEBUG, msg);
}

#endif
