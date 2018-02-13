#include "NetworkImpl.hpp"
#include "NetworkComm.hpp"

namespace zia::network 
{
	NetworkImpl::NetworkImpl()
	{
	}

	NetworkImpl::~NetworkImpl()
	{
		stop();
	}

	bool NetworkImpl::config(zia::api::Conf const & conf)
	{
		// TODO
		return false;
	}

	bool NetworkImpl::run(zia::api::Net::Callback cb)
	{
		if (m_thread || m_running) {
			return false;
		}
		bool	rc = true;
		try {
			NetworkComm netComm(std::move(cb), 4242); // TODO: pass config port
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

	bool NetworkImpl::send(zia::api::ImplSocket* sock, zia::api::Net::Raw const & resp)
	{
		if (!m_running || !m_thread) {
			return false;
		}
		// TODO: push to queue and wait for send ?
		return false;
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
			fd_set readfds, writefds, exceptfds;
			std::int32_t const rc =
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