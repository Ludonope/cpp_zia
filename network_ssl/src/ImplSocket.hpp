#pragma once

#include "Network.hpp"

namespace zia::api
{
	struct ImplSocket
	{
		SSL *		sockSSL;
		network::sock_t	sock;
	};
}
