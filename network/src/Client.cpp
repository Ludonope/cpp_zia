#include <string>
#include "Client.hpp"

namespace zia::network
{
	Client::Client(sock_t const socket, 
			sockaddr_in_t const &sockaddr) :
		m_implSocket{socket},
		m_infos{
			std::chrono::system_clock::now(),
			std::chrono::steady_clock::now(),
			{
				sockaddr.sin_addr.s_addr,
				std::string(inet_ntoa(sockaddr.sin_addr))
			}, ntohs(sockaddr.sin_port), &m_implSocket}
	{
	}

	Client::~Client()
	{
		if (m_implSocket.sock != -1)
		{
			closesocket(m_implSocket.sock);
			m_implSocket.sock = -1;
		}
	}

	Client::Client(Client const &other) :
		m_implSocket(other.m_implSocket),
		m_infos(other.m_infos)
	{
		m_infos.sock = &m_implSocket;
	}

	Client::Client(Client &&other) :
		m_implSocket(std::move(other.m_implSocket)),
		m_infos(std::move(other.m_infos))
	{
		m_infos.sock = &m_implSocket;
		other.m_implSocket.sock = -1;
	}

	Client &Client::operator=(Client const &other)
	{
		if (this != &other)
		{
			m_implSocket = other.m_implSocket;
			m_infos = other.m_infos;
			m_infos.sock = &m_implSocket;
		}
		return *this;
	}

	Client &Client::operator=(Client &&other)
	{
		if (this != &other)
		{
			m_implSocket = std::move(other.m_implSocket);
			m_infos = std::move(other.m_infos);
			m_infos.sock = &m_implSocket;
			other.m_implSocket.sock = -1;
		}
		return *this;
	}

	bool Client::handleInput() noexcept
	{
		// TODO
		return false;
	}

	bool Client::handleOutput() const noexcept
	{
		// TODO
		return false;
	}
}
