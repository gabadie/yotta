#ifndef _H_TESTLIB_UTILS
#define _H_TESTLIB_UTILS

#include <stdio.h>
#include <yotta.h>
#include <mk_test.h>


/*
 * @infos: tests' logger entry
 *
 * @params: See yotta_logger_entry_t in yotta_logger.h
 */
static void
yotta_test_helper_log(yotta_log_msg_type_t msg_type, char const * msg, void * user_data __attribute__((unused)))
{
    static char const * const msg_type_name[] = {
        "ERROR",
        "WARNING",
        "DEBUG"
    };

    test_assert(msg_type < (sizeof(msg_type) / sizeof(typeof(msg_type))));

    printf("%s: %s\n", msg_type_name[msg_type], msg);
}

/*
 * @infos: sets up the yotta test environment
 */
static void
yotta_test_helper_setup()
{
    char const * const exec_name = __FILE__;

    yotta_set_logger_entry(&yotta_test_helper_log, 0);

    test_assert(yotta_init(1, &exec_name) == 0);
}


#endif
