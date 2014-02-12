
#include "../yotta_debug.h"
#include "yotta_str_utils.h"


uint64_t
yotta_str_dec_to_ui64(uint64_t * out_number, char const * str)
{
    yotta_assert(out_number != 0);
    yotta_assert(str != 0);

    uint64_t number = 0;
    char c = *str;

    if (c == 0)
    {
        return -1;
    }

    if (c == '0')
    {
        if (str[1] == 0)
        {
            *out_number = 0;
            return 0;
        }

        return -1;
    }

    do
    {
        if (c < '0' || c > '9')
        {
            return -1;
        }

        uint64_t new_number = number * 10 + (c - '0');

        if (new_number < number)
        {
            // overflow
            return -1;
        }

        number = new_number;

        str++;
        c = *str;
    }
    while (c);

    *out_number = number;

    return 0;
}