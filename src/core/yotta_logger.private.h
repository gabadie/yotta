#ifndef _YOTTAPRIVATE_LOGGER
#define _YOTTAPRIVATE_LOGGER


/*
 * @infos: append an error message to the log
 *
 * @param <msg>: error message to log
 */
void
yotta_logger_error(char const * msg);

/*
 * @infos: append a warning message to the log
 *
 * @param <msg>: warning message to log
 */
void
yotta_logger_warning(char const * msg);

/*
 * @infos: append a debug message to the log
 *
 * @param <msg>: debug message to log
 */
#ifdef YOTTA_DEBUG
void
yotta_logger_debug(char const * msg);
#else
#define yotta_logger_debug(msg)
#endif


#endif
