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

		NetworkComm(NetworkComm const &) = delete;
		NetworkComm &operator=(NetworkComm const &) = delete;

		NetworkComm(NetworkComm &&);
		NetworkComm &operator=(NetworkComm &&);

		void send(api::ImplSocket *, api::Net::Raw &&) noexcept;
		std::int32_t multiplex(fd_set &readfds,
				fd_set &writefds,
				fd_set &exceptfds) const noexcept;
		void handle(fd_set const &readfds,
				fd_set const &writefds,
				fd_set const &exceptfds);

	private:
		api::Net::Callback			m_respCallback = nullptr;
		std::uint16_t				m_port;
		sock_t					m_socket = -1;
		sockaddr_in_t				m_sockAddr = {};
		std::vector<std::unique_ptr<Client>>	m_clients;

#if defined(_WIN32)
		static std::uint32_t	m_nbSockets;
		static bool		m_WSAInited;
		void initWindows() const;
		void deinitWindows() const noexcept;
#endif

		void acceptClient();
	};
}
