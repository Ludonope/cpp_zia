#include <iostream>
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
		bool success = true;
		try
		{
			m_port = std::get<long long>(conf.at("port").v);
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
			success = false;
			m_port = 8080; // Default port value
		}
		try
		{
			m_cert = std::get<std::string>(conf.at("cert").v);
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
			success = false;
			m_cert = "./zia.crt"; // Default certificate value
		}
		try
		{
			m_key = std::get<std::string>(conf.at("key").v);
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
			success = false;
			m_key = "./zia.key"; // Default key value
		}
		return success;
	}

	bool NetworkImpl::run(zia::api::Net::Callback cb)
	{
		std::cout << "Starting server..." << std::endl;
		if (m_thread || m_running) {
			return false;
		}
		bool	rc = true;
		try {
			NetworkComm netComm(std::move(cb), m_port, m_cert, m_key);
			m_thread = std::make_unique<std::thread>(
				[comm = std::move(netComm), this]() mutable {
					this->execute(std::move(comm)); 
				}
			);
			m_running = true;
		}
		catch (std::exception const &e) {
			std::cerr << e.what() << std::endl;
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
		std::cout << "Stopping server" << std::endl;
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
		std::cout << "Server running on port " << m_port << std::endl;
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
