\
#include <stdio.h>

#include "testhelper_init.h"
#include "../src/utils/yotta_file.h"


void
test_file_size()
{
    FILE * file = fopen(__FILE__, "rb");

    {
        test_assert(yotta_file_pos(file) == 0);
        test_assert(yotta_file_size(file) != 0);
        test_assert(yotta_file_pos(file) == 0);
    }

    {
        const size_t pos = 12;

        fseek(file, pos, SEEK_CUR);

        test_assert(yotta_file_pos(file) == pos);
        test_assert(yotta_file_size(file) != 0);
        test_assert(yotta_file_pos(file) == pos);
    }

    fclose(file);
}

int
main()
{
    testhelper_init();

    test_file_size();

    return 0;
}
