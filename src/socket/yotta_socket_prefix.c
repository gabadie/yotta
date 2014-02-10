
#include "yotta_socket_prefix.h"


#ifdef _WIN32 // Windows (x64 and x86)

static WSADATA WSAData;

uint64_t
yotta_init_csocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		return -1;
	}

	return 0;
}

uint64_t
yotta_stop_csocket()
{
	if (WSACleanup())
	{
		return -1;
	}

	return 0;
}


#else // unix

uint64_t
yotta_init_csocket()
{
	return 0;
}

uint64_t
yotta_stop_csocket()
{
	return 0;
}


#endif
