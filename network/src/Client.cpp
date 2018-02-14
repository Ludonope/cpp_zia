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
		m_infos(other.m_infos),
		m_toSend(other.m_toSend)
	{
		m_infos.sock = &m_implSocket;
	}

	Client::Client(Client &&other) :
		m_implSocket(std::move(other.m_implSocket)),
		m_infos(std::move(other.m_infos)),
		m_toSend(std::move(other.m_toSend))
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
			m_toSend = other.m_toSend;
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
			m_toSend = std::move(other.m_toSend);
		}
		return *this;
	}

	bool Client::handleInput() noexcept
	{
		auto const sock = m_implSocket.sock;
		ssize_t rc = 0;
		std::array<std::byte, detail::HTTP_BUFFER_SIZE> buffer = {};

		do
		{
			rc = read(sock, buffer.data(), sizeof(buffer) - 1);
		} while (rc == -1 && errno == EINTR);
		if (rc <= 0) {
			return false;
		}
		m_buffer.write(buffer.data(), rc);
		return true;
	}

	bool Client::handleOutput() noexcept
	{
		auto const sock = m_implSocket.sock;
		auto const data = m_toSend.front();
		std::byte const * const dataPtr = &data[0];
		ssize_t size = data.size() * sizeof(data[0]);
		auto sizeSent = 0;

		while (sizeSent < size)
		{
			ssize_t rc = 0;
			do
			{
				rc = ::write(sock, dataPtr + sizeSent,
					size - sizeSent);
			} while (rc == -1 && errno == EINTR);
			if (rc == -1) {
				return false;
			}
			sizeSent += rc;
		}
		m_toSend.pop();
		return true;
	}
}
