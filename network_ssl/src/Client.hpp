#pragma once

#include "api/net.h"
#include "Network.hpp"
#include "ImplSocket.hpp"
#include "HttpRingBuffer.hpp"
#include "Timer.hpp"
#include <queue>
#include <memory>

namespace zia::network {
	class Client final
	{
	public:
		enum class Status : std::uint8_t
		{
			OK,
			DONE,
			ERR
		};

		Client(sock_t const socket, SSL_CTX *ctx, sockaddr_in_t const &sockaddr);
		~Client();

		Client(Client const &) = delete;
		Client(Client &&) = delete;
		Client &operator=(Client const &) = delete;
		Client &operator=(Client &&) = delete;

		inline bool canWrite() const noexcept
		{
			return !m_toSend.empty();
		}

		inline sock_t getSocket() const noexcept
		{
			return m_implSocket.sock;
		}

		inline bool hasTimedOut() const noexcept
		{
			return m_aliveTimer.check();
		}

		inline zia::api::NetInfo const &getInfos() const noexcept
		{
			return m_infos;
		}

		inline bool hasRequest() const noexcept
		{
			return m_buffer->hasRequest();
		}

		inline zia::api::Net::Raw getRaw() noexcept
		{
			return m_buffer->getRequest();
		}

		Client::Status handleInput() noexcept;
		Client::Status handleOutput() noexcept;

		inline void send(api::Net::Raw &&data)
		{
			m_toSend.push(std::move(data));
		}

	private:
		zia::api::ImplSocket			m_implSocket;
		zia::api::NetInfo			m_infos;
		std::queue<api::Net::Raw>		m_toSend;
		std::unique_ptr<HttpRingBuffer>		m_buffer;
		mutable Timer<std::chrono::seconds>	m_aliveTimer = 120;

		static constexpr auto READ_SIZE = 4096;
		static_assert(READ_SIZE <= detail::HTTP_BUFFER_SIZE);
	};
}
