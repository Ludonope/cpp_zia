#if defined(_WIN32)
# include <io.h>
# include <BaseTsd.h>
#endif

#include <string>
#include "Client.hpp"

namespace zia::network
{
	Client::Client(sock_t const socket, 
			SSL_CTX	*ctx,
			sockaddr_in_t const &sockaddr) :
		m_implSocket{SSL_new(ctx), socket},
		m_infos{
			std::chrono::system_clock::now(),
			std::chrono::steady_clock::now(),
			{
				sockaddr.sin_addr.s_addr,
				std::string(inet_ntoa(sockaddr.sin_addr))
			}, ntohs(sockaddr.sin_port), &m_implSocket},
		m_toSend(),
		m_buffer(std::make_unique<HttpRingBuffer>())
	{
		if (!m_implSocket.sockSSL)
		{
			throw std::runtime_error("Invalid SSL socket");
		}
		SSL_set_fd(m_implSocket.sockSSL, m_implSocket.sock);
		if (SSL_accept(m_implSocket.sockSSL) != 1)
		{
			throw std::runtime_error("SSL handshake failed");
		}
	}

	Client::~Client()
	{
		if (m_implSocket.sockSSL != nullptr)
		{
			SSL_shutdown(m_implSocket.sockSSL);
			SSL_free(m_implSocket.sockSSL);
			m_implSocket.sockSSL = nullptr;
			m_implSocket.sock = -1;
		}
	}

	Client::Status Client::handleInput() noexcept
	{
		auto const sock = m_implSocket.sockSSL;
		ssize_t rc = 0;
		std::array<std::byte, Client::READ_SIZE> buffer = {};

		do
		{
			rc = SSL_read(sock, reinterpret_cast<char *>(buffer.data()), buffer.size() - 1);
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
		auto const sock = m_implSocket.sockSSL;
		auto const data = m_toSend.front();
		std::byte const * const dataPtr = &data[0];
		ssize_t size = data.size() * sizeof(data[0]);
		auto sizeSent = 0;

		while (sizeSent < size)
		{
			ssize_t rc = 0;
			do
			{
				rc = SSL_write(sock, reinterpret_cast<char const * const>(dataPtr) + sizeSent,
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
