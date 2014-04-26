#ifndef _YOTTA_VIRTUAL
#define _YOTTA_VIRTUAL

/*
 * Gets an object's virtual table
 *
 * @param <this> is the pointer on the object
 * @param <vtable_type> is type of the virtual table to cast to
 */
#define yotta_vtable(this,vtable_type) \
    (*(vtable_type const **) (this))

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
#define yotta_vcall(this,vtable_type,function_name,...) \
    (yotta_vfunc(this, vtable_type)(this, __VA_ARGS__))


#endif
