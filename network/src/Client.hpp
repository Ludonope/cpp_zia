#pragma once

#include "api/net.h"
#include "Network.hpp"
#include "ImplSocket.hpp"

namespace zia::network {
	class Client final
	{
	public:
		Client(sock_t const socket, sockaddr_in_t const &sockaddr);
		~Client();

		Client(Client const &);
		Client(Client &&);
		Client &operator=(Client const &);
		Client &operator=(Client &&);

		bool canWrite() const noexcept;
		sock_t getSocket() const noexcept
		{
			return m_implSocket.sock;
		}
		bool hasTimedOut() const noexcept;

		inline zia::api::NetInfo const &getInfos() const noexcept
		{
			return m_infos;
		}

		zia::api::Net::Raw const &getRaw() const noexcept;

		bool handleInput() noexcept;
		bool handleOutput() const noexcept;

	private:
		zia::api::ImplSocket	m_implSocket;
		zia::api::NetInfo	m_infos;
	};
}
