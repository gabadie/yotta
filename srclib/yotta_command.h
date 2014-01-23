#ifndef _YOTTA_COMMAND
#define _YOTTA_COMMAND

#include "yotta_context.h"


/*
 * @infos: store all command's information
 */
typedef
struct yotta_command_s
{
    void * args;
    uint64_t args_size;

} yotta_command_t;


/*
 * @infos: init the yotta library
 *
 * @param <command>: the yotta command to initialize
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_command_init(yotta_command_t * command);

/*
 * @infos: execute the command
 *
 * @param <context>: the yotta context tu executes the command
 * @param <command>: the yotta command to execute
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_command(const yotta_context_t * context, const yotta_command_t * command);

#endif
