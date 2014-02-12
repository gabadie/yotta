

#include <mk_test.h>
#include "../src/yotta_logger.h"
#include "../src/yotta_logger.private.h"

static yotta_log_msg_type_t g_msg_type;


void
test_slave_parse_parameters(yotta_log_msg_type_t msg_type, char const * msg, void * user_data)
{
    test_assert(((uint64_t)g_msg_type) == 0xABC);
    test_assert(user_data == (void *)&g_msg_type);
    test_assert(msg != 0);

    g_msg_type = msg_type;
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
