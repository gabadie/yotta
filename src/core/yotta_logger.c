
#include "yotta_logger.h"
#include "yotta_logger.private.h"
#include "../massive/yotta_dispatch.h"
#include "../threading/yotta_threading.h"


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
void
yotta_logger(yotta_log_msg_type_t msg_type, char const * msg)
{
    if (yotta_logger_callback == 0)
    {
        return;
    }

    yotta_logger_param_t logger_param;

    logger_param.type = msg_type;
    logger_param.msg = msg;
    logger_param.process_id = yotta_threading_pid();
    logger_param.thread_id = yotta_threading_tid();

    yotta_get_local_id(&logger_param.local_id, &logger_param.local_count);
    yotta_get_group_id(&logger_param.group_id, &logger_param.group_count);
    yotta_get_global_id(&logger_param.global_id, &logger_param.global_count);

    yotta_logger_callback(yotta_logger_user_data, &logger_param);
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
