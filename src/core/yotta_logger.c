
#include "yotta_logger.h"
#include "yotta_logger.private.h"


// global logger's callback
yotta_logger_entry_t yotta_logger_callback = 0;

// global logger's user data
void * yotta_logger_user_data;


/*
 * @infos: log out a message
 *
 * @param <msg_type>: message type
 * @param <msg>: message content
 *
 * @returns: void
 */
#define yotta_logger(msg_type,msg) \
    if (yotta_logger_callback) \
    { \
        yotta_logger_callback(msg_type, msg, yotta_logger_user_data); \
    }


yotta_return_t
yotta_set_logger_entry(yotta_logger_entry_t callback, void * user_data)
{
    yotta_logger_callback = callback;
    yotta_logger_user_data = user_data;

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
