#ifndef _YOTTA_DEBUG
#define _YOTTA_DEBUG

#include "yotta_struct.h"
#include "../yotta_prefix.h"

#ifdef YOTTA_DEBUG
#include <stdio.h>
#include <string.h>

#ifndef YOTTA_ASSERT
#define YOTTA_ASSERT
#endif

#define YOTTA_DEBUG_DIRTY

#endif


/*
 * @infos: generates a debug breakpoint when reached
 */
#ifdef YOTTA_DEBUG
#define yotta_break_point() \
    __builtin_trap()

#else
#define yotta_break_point()

#endif

/*
 * @infos: use yotta_crash instead
 */
#define yotta_crash_definitly() \
    do { \
        yotta_break_point(); \
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
        printf("CRASH AT %s@%i in %s: ", __FILE__, __LINE__, __func__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        yotta_crash_definitly(); \
    }

#else
#define yotta_crash_msg(...)
#endif

/*
 * @infos: definies a todo that crash when reached
 *
 * @params: like printf
 */
#ifdef YOTTA_DEBUG
#define yotta_todo(...) \
    { \
        printf("TODO AT %s@%i in %s: ", __FILE__, __LINE__, __func__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        yotta_crash_definitly(); \
    }

#else
#define yotta_todo(...)
#endif

/*
 * @infos: assert yotta execution
 *
 * @param <condition>: assertion's condition code to pass
 */
#ifdef YOTTA_ASSERT
#define yotta_assert(condition) \
    if (!(condition)) \
    { \
        printf("ASSERTION ERROR AT %s@%i in %s: %s\n", __FILE__, __LINE__, __func__, yotta_to_str(condition)); \
        yotta_crash_definitly(); \
    }

#else //YOTTA_ASSERT
#define yotta_assert(condition)

#endif //YOTTA_ASSERT

/*
 * @infos: assert yotta execution
 *
 * @param <condition>: assertion's returned value
 */
#ifdef YOTTA_ASSERT
#define yotta_assert_return(function,returned) \
    if ((function) != (returned)) \
    { \
        printf("ASSERTION ERROR AT %s@%i in %s: %s\n", __FILE__, __LINE__, __func__, yotta_to_str(function) " == " yotta_to_str(returned)); \
        yotta_crash_definitly(); \
    }

#else //YOTTA_ASSERT
#define yotta_assert_return(function,returned) \
    function

#endif //YOTTA_ASSERT

/*
 * @infos: not implemented feature crash
 */
#ifdef YOTTA_DEBUG
#define yotta_not_implemented_yet() \
    yotta_crash_msg("Not implemented yet");
#else
#define yotta_not_implemented_yet()
#endif

/*
 * @infos: soils a memory block
 *
 * @param <dest>: the memory block to soil
 * @param <size>: the memory block's size
 *
 */
#ifdef YOTTA_DEBUG_DIRTY
#define yotta_dirty(dest,size) \
    memset((dest), 0xFF, (size))

#else
#define yotta_dirty(dest,size)

#endif

/*
 * @infos: soils a memory block by its pointing type
 *
 * @param <dest>: the memory block to soil
 * @param <offset>: the offset memory block's
 * @param <size>: the memory block's size
 */
#define yotta_dirty_offset(dest,offset,size) \
    yotta_dirty(((uint8_t *) (dest)) + (offset), (size))

/*
 * @infos: soils a memory block by its pointing type
 *
 * @param <dest>: the memory block to soil
 */
#define yotta_dirty_s(dest) \
    yotta_dirty((dest), sizeof(*(dest)))

/*
 * @infos: log a message
 *
 * @params: like printf
 *
 */
#ifdef YOTTA_DEBUG
#define yotta_log(...) \
    fprintf(stderr, __VA_ARGS__)
#else
#define yotta_log(...)
#endif

/*
 * @infos: log a message
 *
 * @params <msg>: message to log
 *
 */
#ifdef YOTTA_DEBUG
#define yotta_perror(...) \
    perror(__VA_ARGS__)
#else
#define yotta_perror(...)
#endif


#endif
