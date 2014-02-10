#ifndef _YOTTA_DEBUG
#define _YOTTA_DEBUG

#include "yotta_prefix.h"

#ifdef YOTTA_DEBUG
#include <stdio.h>
#endif


/*
 * @infos: use yotta_crash instead
 */
#define yotta_crash_definitly() \
    do { \
        *((volatile int*) NULL) = 123; \
    } while (0)


/*
 * @infos: convert code to string
 */
#define yotta_to_str(code) #code


/*
 * @infos: crashes yotta execution
 */
#ifdef YOTTA_DEBUG
#define yotta_crash() \
    { \
        printf("CRASH AT %s@%i\n", __FILE__, __LINE__); \
        yotta_crash_definitly(); \
    }

#else
#define yotta_crash()
#endif


/*
 * @infos: crashes yotta execution with a message
 *
 * @params: like printf
 */
#ifdef YOTTA_DEBUG
#define yotta_crash_msg(...) \
    { \
        printf("CRASH AT %s@%i: ", __FILE__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        yotta_crash_definitly(); \
    }

#else
#define yotta_crash_msg(...)
#endif

/*
 * @infos: crashes yotta execution with a message
 *
 * @param <condition>: assertion's condition code to pass
 */
#ifdef YOTTA_DEBUG
#define yotta_assert(condition) \
    if (!(condition)) \
    { \
        printf("ASSERTION ERROR AT %s@%i: %s\n", __FILE__, __LINE__, yotta_to_str(condition)); \
        yotta_crash_definitly(); \
    }
#else
#define yotta_assert(condition)
#endif


#endif
