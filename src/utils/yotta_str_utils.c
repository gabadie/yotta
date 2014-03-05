
#include "yotta_str_utils.h"
#include "../core/yotta_debug.h"


/*
 * Stores all digits
 */
static
char const yotta_str_digit[] = "0123456789abcdef";

yotta_return_t
yotta_str_to_ui64(uint64_t * out_number, char const * str, uint64_t basis)
{
    yotta_assert(out_number != 0);
    yotta_assert(str != 0);
    yotta_assert(basis == 0 || (basis > 1 && basis < sizeof(yotta_str_digit)));

    uint64_t number = 0;
    char c = *str;

    if (c == 0)
    {
        return YOTTA_INVALID_VALUE;
    }

    if (c == '0')
    {
        if (str[1] == 0)
        {
            *out_number = 0;
            return 0;
        }
        else if (basis != 0)
        {
            return YOTTA_INVALID_VALUE;
        }

        if (str[1] == 'x')
        {
            basis = 16;
            str += 2;
        }
        else if (str[1] == 'b')
        {
            basis = 2;
            str += 2;
        }
        else if (str[1] >= '0' && str[1] < '8')
        {
            basis = 8;
            str++;
        }
        else
        {
            return YOTTA_INVALID_VALUE;
        }

        c = *str;

        if (c == '0')
        {
            if (str[1] == 0)
            {
                *out_number = 0;
                return 0;
            }

            return YOTTA_INVALID_VALUE;
        }
    }

    if (basis == 0)
    {
        basis = 10;
    }

    do
    {
        uint64_t digit = ((uint64_t) c) + (uint64_t)(10 - 'a');

        if (c >= '0' && c <= '9')
        {
            digit = c - '0';
        }
        else if (c >= 'A' && c <= 'Z')
        {
            digit = ((uint64_t) c) + (uint64_t)(10 - 'A');
        }

        if (digit >= basis)
        {
            return YOTTA_INVALID_VALUE;
        }

        uint64_t new_number = number * basis + digit;

        if (new_number < number)
        {
            // overflow
            return YOTTA_INVALID_VALUE;
        }

        number = new_number;

        str++;
        c = *str;
    }
    while (c);

    *out_number = number;

    return YOTTA_SUCCESS;
}

void
yotta_ui64_to_str(char * out_str, uint64_t number, uint64_t basis)
{
    yotta_assert(out_str != 0);
    yotta_assert(basis > 1 && basis < sizeof(yotta_str_digit));

    if (number == 0)
    {
        out_str[0] = yotta_str_digit[0];
        out_str[1] = 0;
        return;
    }

    uint64_t divisor = basis;

    for ( ; ; )
    {
        if ((number / divisor) == 0)
        {
            break;
        }

        divisor *= basis;
    }

    divisor /= basis;

    do
    {
        uint64_t digit = (number / divisor);

        *out_str = yotta_str_digit[digit];
        out_str++;

        number -= digit * divisor;
        divisor /= basis;
    }
    while (divisor != 0);

    *out_str = 0;
}
