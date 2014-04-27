/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */

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
