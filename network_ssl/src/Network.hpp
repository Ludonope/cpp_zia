#pragma once

#if defined __linux__ || defined __APPLE__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#if defined closesocket

#warning "closesocket is already defined"
#undef closesocket

#endif

#define closesocket(s) close(s)

#elif defined _WIN32

// Prevent inclusion of winsock.h in windows.h
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <windows.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#endif

namespace zia::network
{
#if defined _WIN32
		using sock_t = SOCKET;
		using sockaddr_in_t = SOCKADDR_IN;
		using sockaddr_in6_t = SOCKADDR_IN6;
		using sockaddr_t = SOCKADDR;
		using _in_addr_t = IN_ADDR;
		using addrinfo_t = struct addrinfo;
		using ssize_t = SSIZE_T;
#else
		using sock_t = int;
		using sockaddr_in_t = struct sockaddr_in;
		using sockaddr_in6_t = struct sockaddr_in6;
		using sockaddr_t = struct sockaddr;
		using _in_addr_t = struct in_addr;
		using addrinfo_t = struct addrinfo;
#endif
}

#include <openssl/ssl.h>
#include <openssl/err.h>
