
#include "yotta_init.h"
#include "yotta_address_translation.h"

char const *
yotta_executable_path = 0;


uint64_t
yotta_init(uint64_t argc __attribute__((unused)), char * const * const argv)
{
    yotta_executable_path = argv[0];

    return 0;
}
