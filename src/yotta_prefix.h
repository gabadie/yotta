#ifndef _YOTTA_PREFIX
#define _YOTTA_PREFIX

#include <stdint.h>
#include <stdlib.h>

/*
 * Defines compilation environment YOTTA_{LINUX,OSX,UNIX,WINDOWS}
 */
#ifdef __APPLE__
#define YOTTA_OSX
#define YOTTA_UNIX
#endif

#ifdef __linux__
#define YOTTA_LINUX
#define YOTTA_UNIX
#endif

#if defined(_WIN32) || defined(_WIN64)
#define YOTTA_WINDOWS
#endif


/*
 * We uses the POSIX standart on UNIX compatible system
 */
#ifdef YOTTA_UNIX
#define YOTTA_POSIX
#endif


#include "core/yotta_return.h"


#endif
