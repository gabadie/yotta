#ifndef _YOTTA_CLIB_SOCKET
#define _YOTTA_CLIB_SOCKET


#ifdef _WIN32 // Windows (x64 and x86)

#define NOMINMAX
#include <winsock2.h>
#include <windows.h>
typedef int socklen_t;


#elif __unix__ || __linux__ || __APPLE__ // unix

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket(a) close(a)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;


#endif

#ifndef NBCLIENT
#define NBCLIENT 10
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

/*
 * @infos: inits C socket library
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_init_csocket();

/*
 * @infos: cleans C socket library
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
uint64_t
yotta_clean_csocket();


#endif
