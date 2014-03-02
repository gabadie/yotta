#ifndef _YOTTAPRIVATE_SLAVE
#define _YOTTAPRIVATE_SLAVE

#include "../yotta_prefix.h"


/*
 * @infos: yotta slave parameters
 */
typedef struct
yotta_slave_parameters_s
{
    // client ip string
    char const * client_ip;

    // client port
    uint16_t client_port;
} yotta_slave_parameters_t;


/*
 * @infos: Slave parameters parser.
 *
 * @parameters:
 *  --yotta-client-ip 192.168.1.42 --yotta-client-port 1234
 *
 * @param <out_parameters>: parsed slave parameters
 *
 * @returns:
 *  == <0> if succed
 *  != <0> if failed
 */
// static: for testing (in ../tests/testlib_init.c)
uint64_t
yotta_slave_parse_parameters(yotta_slave_parameters_t * out_parameters, uint64_t argc, char const * const * argv);

#endif
