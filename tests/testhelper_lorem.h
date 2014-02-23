#ifndef _TESTHELPER_LOREM
#define _TESTHELPER_LOREM

#include <stdlib.h>


static
void
testhelper_lorem(void * data_dest, uint64_t data_size)
{
    uint32_t * cursor = (uint32_t *) data_dest;
    uint32_t * cursor_end = (uint32_t *)(((uint8_t *) cursor) + data_size);

    while(cursor < cursor_end)
    {
        *cursor = (uint32_t) rand();
        cursor++;
    }
}


#endif
