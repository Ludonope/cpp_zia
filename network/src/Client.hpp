#pragma once

#include "api/net.h"
#include "Network.hpp"
#include "ImplSocket.hpp"
#include <queue>

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

		inline bool canWrite() const noexcept
		{
			return !m_toSend.empty();
		}

		inline sock_t getSocket() const noexcept
		{
			return m_implSocket.sock;
		}

		inline bool hasTimedOut() const noexcept
		{
			// TODO
			return false;
		}

		inline zia::api::NetInfo const &getInfos() const noexcept
		{
			return m_infos;
		}

		zia::api::Net::Raw const &getRaw() const noexcept;

		bool handleInput() noexcept;
		bool handleOutput() noexcept;

		inline void send(api::Net::Raw &&data)
		{
			m_toSend.push(std::move(data));
		}

	private:
		zia::api::ImplSocket		m_implSocket;
		zia::api::NetInfo		m_infos;
		std::queue<api::Net::Raw>	m_toSend;
	};
}
