
#include "yotta_debug.h"
#include "yotta_init.h"
#include "yotta_address_translation.h"
#include "socket/yotta_socket_prefix.h"

char const *
yotta_executable_path = 0;


uint64_t
yotta_init(uint64_t argc __attribute__((unused)), char * const * const argv)
{
    yotta_assert(argc > 1);
    yotta_assert(argv != 0);

    yotta_executable_path = argv[0];

    if (yotta_init_csocket())
    {
        return -1;
    }

    return 0;
}
