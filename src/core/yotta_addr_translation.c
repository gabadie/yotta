#include "yotta_addr_translation.private.h"


static void
yotta_base_function() { }

static const uint64_t
yotta_base_address = (uint64_t) &yotta_base_function;


yotta_rel_addr_t
yotta_address_absolute_to_relative(uint64_t address)
{
    return (yotta_rel_addr_t) address - yotta_base_address;
}

uint64_t
yotta_address_relative_to_absolute(yotta_rel_addr_t offset)
{
    return (uint64_t) yotta_base_address + (uint64_t) offset;
}
