#include "NetworkImpl.hpp"
#include "NetworkComm.hpp"

namespace zia::network 
{
	NetworkImpl::NetworkImpl() : m_toSend()
	{
	}

	NetworkImpl::~NetworkImpl()
	{
		stop();
	}

	bool NetworkImpl::config(zia::api::Conf const & conf)
	{
		// TODO
		if (m_port == 0)
		{
			m_port = 8080; // Default port value
		}
		return false;
	}

	bool NetworkImpl::run(zia::api::Net::Callback cb)
	{
		if (m_thread || m_running) {
			return false;
		}
		bool	rc = true;
		try {
			NetworkComm netComm(std::move(cb), m_port);
			m_thread = std::make_unique<std::thread>(
				[&](){ this->execute(std::move(netComm)); }
			);
			m_running = true;
		}
		catch (std::exception const &) {
			// TODO: Log ?
			rc = false;
		}
		return rc;
	}

	bool NetworkImpl::send(api::ImplSocket* sock, api::Net::Raw const & resp)
	{
		if (!m_running || !m_thread) {
			return false;
		}
		m_toSend.push({sock, resp});
		return true;
	}

	bool NetworkImpl::stop()
	{
		if (!m_running || !m_thread) {
			return false;
		}
		m_running = false;
		if (m_thread->joinable()) {
			m_thread->join();
		}
		m_thread = nullptr;
		return true;
	}

	void NetworkImpl::execute(NetworkComm &&netComm) noexcept
	{
		while (m_running)
		{
			while (!m_toSend.empty())
			{
				auto [sock, data] = m_toSend.pop();
				netComm.send(sock, std::move(data));
			}

			fd_set readfds, writefds, exceptfds;
			auto const rc =
				netComm.multiplex(readfds, writefds,
							exceptfds);
			if (rc < 0)
			{
				break;
			}
			else if (rc)
			{
				netComm.handle(readfds, writefds, exceptfds);
			}
		}
	}
}