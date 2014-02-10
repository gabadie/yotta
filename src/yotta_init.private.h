#ifndef _YOTTAPRIVATE_INIT
#define _YOTTAPRIVATE_INIT

#include "yotta_prefix.h"

/*
 * @infos: yotta command parameters
 */
typedef struct
yotta_cmd_parameters_s
{
    // client ip string
    char const * client_ip;

    // client port
    uint16_t client_port;

    // massive function pointer
    uint64_t massive_function_offset;
} yotta_cmd_parameters_t;


// static: for testing (in ../tests/testlib_init.c)
uint64_t
yotta_init_slave_parse(yotta_cmd_parameters_t * out_parameters, uint64_t argc, char const * const * argv);


#endif
