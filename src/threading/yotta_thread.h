#ifndef _YOTTA_THREAD
#define _YOTTA_THREAD


/*
 * Defines the yotta thread local prefix
 */
// yotta_thread_local

#if defined(__GNUC__) // GCC, clang ...
#define yotta_thread_local __thread

#elif defined(_WIN32) || defined(_WIN64) // Visual studio
#define yotta_thread_local __declspec(thread)

#else // unknown compilor
#error "yotta_thread_local unsupported"

#endif


/*
 * @infos: defines a yotta thread struct
 */
// yotta_thread_t

/*
 * @infos: creates a yotta thread
 *
 * @param <thread_ptr>: pointer on the yotta thread
 * @param <funct_ptr>: thread function pointer
 * @param <param_ptr>: pointer parameter to pass to the thread
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
// yotta_thread_create(thread_ptr,funct_ptr,param_ptr)

/*
 * @infos: joins a yotta thread
 *
 * @param <thread>: the yotta thread to join
 */
// yotta_thread_join(thread)

#if defined(_WIN32) || defined(_WIN64) // Windows threads
#include <windows.h>

typedef HANDLE yotta_thread_t;

#define yotta_thread_create(thread_ptr,funct_ptr,param_ptr) \
    ((*(thread_ptr) = CreateThread(0, 0, (funct_ptr), (param_ptr), 0, 0)) == 0)

#define yotta_thread_join(thread) \
    { \
        WaitForSingleObject((thread), INFINITE); \
        CloseHandle((thread)); \
    }


#else // POSIX threads
#include <pthread.h>

typedef pthread_t yotta_thread_t;

#define yotta_thread_create(thread_ptr,funct_ptr,param_ptr) \
    pthread_create((thread_ptr), 0, (void *(*)(void *)) (funct_ptr), (void *) (param_ptr))

#define yotta_thread_join(thread) \
    pthread_join((thread), 0)


#endif


#endif
