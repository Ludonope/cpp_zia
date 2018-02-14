#pragma once

#include "api/net.h"
#include "TSQueue.hpp"
#include <thread>
#include <memory>
#include <atomic>

namespace zia::network 
{
	class NetworkComm;

	class NetworkImpl final : public api::Net 
	{
	public:
		NetworkImpl();
		virtual ~NetworkImpl();
		bool config(api::Conf const & conf) override;
		bool run(api::Net::Callback cb) override;
		bool send(api::ImplSocket* sock, api::Net::Raw const & resp) override;
		bool stop() override;

	private:
		using ClientMsg = std::pair<api::ImplSocket *, api::Net::Raw>;
		std::uint16_t			m_port = 0;
		std::unique_ptr<std::thread>	m_thread = nullptr;
		std::atomic<bool>		m_running = false;
		TSQueue<ClientMsg>		m_toSend;

		void execute(NetworkComm &&) noexcept;
	};
}
