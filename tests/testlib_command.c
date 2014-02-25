
#include <mk_test.h>
#include <yotta.h>
#include <string.h>


void
test_command_init()
{
    yotta_command_t command;
    char const null_command[sizeof(yotta_command_t)] = { 0 };

    test_assert(yotta_command_init(0) == YOTTA_INVALID_VALUE);
    test_assert(yotta_command_init(&command) == YOTTA_SUCCESS);

    test_assert(memcmp(&command, null_command, sizeof(yotta_command_t)) == 0);
}

int
main()
{
    test_command_init();

    return 0;
}
