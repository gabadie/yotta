#ifndef _YOTTA_VIRTUAL
#define _YOTTA_VIRTUAL

#include "yotta_struct.h"


/*
 * Gets an object's virtual table
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table to cast to
 */
#define yotta_vtable(this,vtable_type) \
    (*(vtable_type const **) (this))

/*
 * Sets an object's virtual table
 *
 * @param <this> is the pointer on the object
 * @param <vtable> is the vtable to set
 */
#define yotta_vtable_set(this,vtable) \
    ((*(void const **) (this)) = (void const *)(vtable))

/*
 * Gets an object's virtual function
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table
 * @param <function_name> the name of the virtual function
 */
#define yotta_vfunc(this,vtable_type,function_name) \
    (yotta_vtable(this, vtable_type)->function_name)

/*
 * Calls an object's virtual function
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table
 * @param <function_name> the name of the virtual function
 * @param <...> the virtual function parameters
 *
 * @example:
 *  yotta_vcall(my_object, my_vtable_t, my_function_name, foo, bar)
 */
#define yotta_vcall(this,vtable_type,function_name) \
    (yotta_vfunc(this, vtable_type, function_name)(this))

#define yotta_vcall_params(this,vtable_type,function_name,...) \
    (yotta_vfunc(this, vtable_type, function_name)(this, __VA_ARGS__))

/*
 * Calls an object's virtual release function
 *
 * @param <this> is the pointer on the object to release
 */
#define yotta_vcall_release(this) \
    ((**((void (***)(typeof(this))) (this)))(this))


#endif
