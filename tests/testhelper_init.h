#ifndef _TESTHELPER_INIT
#define _TESTHELPER_INIT

#include <stdio.h>
#include <yotta.h>
#include <mk_test.h>


/*
 * @infos: tests' logger entry
 *
 * @params: See yotta_logger_entry_t in yotta_logger.h
 */
static
void
testhelper_log(void * user_data, yotta_logger_param_t const * param)
{
    static char const * const msg_type_name[] = {
        "ERROR",
        "WARNING",
        "DEBUG"
    };

    test_assert(user_data == 0);
    test_assert(param != 0);
    test_assert(param->type < (sizeof(msg_type_name) / sizeof(typeof(msg_type_name))));

    printf("%s: %s\n", msg_type_name[param->type], param->msg);
}

/*
 * @infos: sets up the yotta test environment
 */
static
void
testhelper_init()
{
    char const * const exec_name = __FILE__;

    yotta_set_logger_entry(&testhelper_log, 0);

    test_assert(yotta_init(1, &exec_name) == 0);
}


#endif
