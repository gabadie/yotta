#ifndef _YOTTA_DISPATCH
#define _YOTTA_DISPATCH

#include "../yotta_prefix.h"


/*
 * @infos: defines yotta dispatch entry function pointer type
 *
 * The tcp command entry received 2 parameters:
 *  - void * param: the user param
 */
typedef void (* yotta_dispatch_func_t)(void *);


/*
 * @infos: dispatches threads on all available cores
 *
 * @param <user_function>: the master address
 * @param <user_param>: the user parameter
 * @param <user_param_stride>: the user parameter pointer stride for each local
 *  thread.
 *
 * @returns:
 *  <YOTTA_SUCESS> if successed
 *  <YOTTA_INVALID_VALUE> if <function> is null
 *  <YOTTA_INVALID_OPERATION> if a dispatch has already been launched
 */
yotta_return_t
yotta_dispatch(yotta_dispatch_func_t user_function, void * user_param, uint64_t user_param_stride);

/*
 * @infos: barrier synchronisation between group's threads
 */
void
yotta_group_barrier();

/*
 * @infos: gets the local thread id and local threads count
 *
 * @param <out_id>: the current thread's id
 * @param <out_count>: the number of threads in the current thread's group
 *
 * @caution: all groups might not have the same number of threads
 */
void
yotta_get_local_id(uint64_t * out_id, uint64_t * out_count);

/*
 * @infos: gets the local thread's group id and threads groups count
 *
 * @param <out_id>: the the current thread's group's id
 * @param <out_count>: the number of threads groups
 */
void
yotta_get_group_id(uint64_t * out_id, uint64_t * out_count);

/*
 * @infos: gets the global thread id and global threads count
 *
 * @param <out_id>: the current thread's id
 * @param <out_count>: the number of threads in the current thread's group
 */
void
yotta_get_global_id(uint64_t * out_id, uint64_t * out_count);


#endif
