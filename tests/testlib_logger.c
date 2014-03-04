

#include <mk_test.h>
#include "../src/core/yotta_logger.h"
#include "../src/core/yotta_logger.private.h"

static yotta_log_msg_type_t g_msg_type;


void
test_slave_parse_parameters(void * user_data, yotta_logger_param_t const * param)
{
    test_assert(((uint64_t)g_msg_type) == 0xABC);
    test_assert(user_data == (void *)&g_msg_type);
    test_assert(param != 0);

    g_msg_type = param->type;
}


int
main()
{
    yotta_logger_error("this is an hidden error message");

    yotta_set_logger_entry(&test_slave_parse_parameters, &g_msg_type);

    g_msg_type = 0xABC;
    yotta_logger_error("this is an error message");
    test_assert(g_msg_type == YOTTA_LOG_ERROR);

    g_msg_type = 0xABC;
    yotta_logger_warning("this is a warning message");
    test_assert(g_msg_type == YOTTA_LOG_WARNING);

    return 0;
}
