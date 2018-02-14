#pragma once

#include <cstdint>

namespace zia::network {
	class NetworkComm final
	{
	public:
		explicit NetworkComm(std::uint16_t const port);
		~NetworkComm();

		NetworkComm(NetworkComm const &) = default;
		NetworkComm(NetworkComm &&) = default;
		NetworkComm &operator=(NetworkComm const &) = default;
		NetworkComm &operator=(NetworkComm &&) = default;

	private:
		std::uint16_t	m_port;
	};
}