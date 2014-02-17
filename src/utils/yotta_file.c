
#include "yotta_file.h"
#include "../yotta_debug.h"


uint64_t
yotta_file_size(FILE * file)
{
    yotta_assert(file != 0);

    uint64_t current_pos = yotta_file_pos(file);

    yotta_assert_return(fseek(file, 0, SEEK_END), 0);

    uint64_t file_size = yotta_file_pos(file);

    yotta_assert_return(fseek(file, current_pos, SEEK_SET), 0);

    return file_size;
}
