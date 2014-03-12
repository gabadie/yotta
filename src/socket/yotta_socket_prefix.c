
#include "yotta_socket_prefix.h"


#ifdef _WIN32 // Windows (x64 and x86)

static WSADATA WSAData;

yotta_return_t
yotta_init_csocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		return YOTTA_UNEXPECTED_FAIL;
	}

	return YOTTA_SUCCESS;
}

yotta_return_t
yotta_clean_csocket()
{
	if (WSACleanup())
	{
		return YOTTA_UNEXPECTED_FAIL;
	}

	return YOTTA_SUCCESS;
}


#else // unix

yotta_return_t
yotta_init_csocket()
{
	return YOTTA_SUCCESS;
}

yotta_return_t
yotta_clean_csocket()
{
	return YOTTA_SUCCESS;
}


#endif
