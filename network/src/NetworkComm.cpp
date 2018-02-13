#include "NetworkComm.hpp"

namespace zia::network
{
	NetworkComm::NetworkComm(std::uint16_t const port) :
		m_port(port)
	{
	}

	NetworkComm::~NetworkComm()
	{
		// TODO
	}
}