#pragma once

#include "api/net.h"
#include "Network.hpp"
#include "Client.hpp"
#include <cstdint>
#include <memory>

#if defined __linux__ || defined __APPLE__
#include <sys/select.h>
#endif

namespace zia::network {
	class NetworkComm final
	{
	public:
		NetworkComm(zia::api::Net::Callback &&,
				std::uint16_t const port);
		~NetworkComm();

		NetworkComm(NetworkComm const &) = default;
		NetworkComm &operator=(NetworkComm const &) = default;

		NetworkComm(NetworkComm &&);
		NetworkComm &operator=(NetworkComm &&);

		std::int32_t multiplex(fd_set &readfds,
				fd_set &writefds,
				fd_set &exceptfds) const noexcept;
		void handle(fd_set const &readfds,
				fd_set const &writefds,
				fd_set const &exceptfds);

	private:
		zia::api::Net::Callback			m_respCallback = nullptr;
		std::uint16_t				m_port;
		sock_t					m_socket = -1;
		sockaddr_in_t				m_sockAddr = {};
		std::vector<std::unique_ptr<Client>>	m_clients;

		void acceptClient();
	};
}
