#include "yotta_address_translation.h"


static void
yotta_base_function() { }

static const uint64_t
yotta_base_address = (uint64_t) &yotta_base_function;


uint64_t
yotta_address_absolute_to_relative(uint64_t address)
{
    return (uint64_t) address - yotta_base_address;
}

uint64_t
yotta_address_relative_to_absolute(uint64_t offset)
{
    return (uint64_t) yotta_base_address + offset;
}
