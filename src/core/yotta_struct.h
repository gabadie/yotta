#ifndef _YOTTA_STRUCT
#define _YOTTA_STRUCT


/*
 * @infos: gets the member's offset in the struct
 */
#define yotta_struct_offset(struct_name, member_name) \
    ((size_t) (&((struct_name *) NULL)->member_name))

/*
 * @infos: gets the container struct pointer
 */
#define yotta_struct_container(struct_name, member_name, ptr) \
    ((struct_name *) (((size_t) ptr) - yotta_struct_offset(struct_name, member_name)))


#endif //_YOTTA_STRUCT
