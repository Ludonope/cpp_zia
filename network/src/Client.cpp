#if defined(_WIN32)
# include <io.h>
# include <BaseTsd.h>
# define read _read
# define write _write
#endif

#include <string>
#include "Client.hpp"

#include <iostream>
#include <exception>

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
				std::string(inet_ntoa(sockaddr.sin_addr)) // TODO: use inet_ntop() instead for IPV6 support
			}, ntohs(sockaddr.sin_port), &m_implSocket},
		m_toSend(),
		m_buffer(std::make_unique<HttpRingBuffer>())
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

	Client::Status Client::handleInput() noexcept
	{
		auto const sock = m_implSocket.sock;
		ssize_t rc = 0;
		std::array<std::byte, detail::HTTP_BUFFER_SIZE> buffer = {};

		do
		{
			rc = read(sock, buffer.data(), buffer.size() - 1);
		} while (rc == -1 && errno == EINTR);
		if (rc == 0)
		{
			return Status::DONE;
		}
		else if (rc == -1)
		{
			return Status::ERR;
		}
		m_buffer->write(buffer.data(), rc);
		return Status::OK;
	}

	Client::Status Client::handleOutput() noexcept
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
			if (rc == 0)
			{
				return Status::DONE;
			}
			else if (rc == -1)
			{
				return Status::ERR;
			}
			sizeSent += rc;
		}
		m_toSend.pop();
		return Status::OK;
	}
}
