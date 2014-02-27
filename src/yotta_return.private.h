#ifndef _YOTTAPRIVATE_RETURN
#define _YOTTAPRIVATE_RETURN

#include "yotta_return.h"
#include "yotta_debug.h"
#include "core/yotta_logger.private.h"


/*
 * @infos: returns YOTTA_INVALID_VALUE
 *
 * @param <function_name>: the function's name
 * @param <arg_name>: the argument's name
 */
#define yotta_return_inv_value(function_name,arg_name) \
    {\
        yotta_logger_error( \
            "INVALID_VALUE: <" \
            yotta_to_str(arg_name) \
            "> of " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_INVALID_VALUE; \
    }

/*
 * @infos: returns YOTTA_INVALID_ENUM
 *
 * @param <function_name>: the function's name
 * @param <arg_name>: the argument's name
 */
#define yotta_return_inv_enum(function_name,arg_name) \
    {\
        yotta_logger_error( \
            "INVALID_ENUMERATION: <" \
            yotta_to_str(arg_name) \
            "> of " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_INVALID_ENUM; \
    }

/*
 * @infos: returns YOTTA_INVALID_OPERATION
 *
 * @param <function_name>: the function's name
 */
#define yotta_return_inv_op(function_name) \
    {\
        yotta_logger_error( \
            "INVALID_OPERATION: " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_INVALID_OPERATION; \
    }

/*
 * @infos: returns YOTTA_UNEXPECTED_FAIL
 *
 * @param <function_name>: the function's name
 */
#define yotta_return_unexpect_fail(function_name) \
    {\
        yotta_logger_error( \
            "UNEXPECTED_FAIL: " \
            yotta_to_str(function_name) \
        ); \
        return YOTTA_UNEXPECTED_FAIL; \
    }

#endif
