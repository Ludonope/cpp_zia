#pragma once

#include "api/net.h"
#include <thread>
#include <memory>
#include <atomic>

namespace zia::network 
{
	class NetworkComm;

	class NetworkImpl final : public zia::api::Net 
	{
	public:
		NetworkImpl();
		virtual ~NetworkImpl();
		bool config(zia::api::Conf const & conf) override;
		bool run(zia::api::Net::Callback cb) override;
		bool send(zia::api::ImplSocket* sock, zia::api::Net::Raw const & resp) override;
		bool stop() override;

	private:
		std::unique_ptr<std::thread>	m_thread = nullptr;
		std::atomic<bool>		m_running = false;

		void execute(NetworkComm &&) noexcept;
	};
}
