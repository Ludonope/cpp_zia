#include "ModuleManager.hpp"

namespace zia::core
{
	ModuleManager::ModuleManager(api::Conf const &conf) :
		m_conf(conf)
	{
	}

	ModuleManager::~ModuleManager()
	{
	}

	void ModuleManager::start()
	{
		configureModuleList(m_receiveModule, m_conf);
		configureModuleList(m_processingModule, m_conf);
		configureModuleList(m_sendingModule, m_conf);
		if (m_networkModule)
		{
			if (!m_networkModule->config(m_conf))
			{
				// TODO: Error during configuration
			}
			m_networkModule->run([this](auto raw, auto infos){
				receiveCallback(std::move(raw), std::move(infos));
			});
		}
	}

	void ModuleManager::stop()
	{
		if (m_networkModule)
		{
			m_networkModule->stop();
		}
	}

	void	ModuleManager::receiveCallback(api::Net::Raw &&raw, api::NetInfo &&infos)
	{
		api::HttpDuplex	duplex = {};

		duplex.info = infos;
		duplex.raw_req = raw;
		// TODO: Parse HTTP here
		auto const moduleProcess = [&](ModuleList &list){
			for (auto &module : list)
			{
				module->exec(duplex);
			}
		};
		moduleProcess(m_receiveModule);
		moduleProcess(m_processingModule);
		moduleProcess(m_sendingModule);
		m_networkModule->send(infos.sock, duplex.raw_resp);
	}

	void	ModuleManager::configureModuleList(ModuleList &list, api::Conf const &conf)
	{
		for (auto &module : list)
		{
			if (!module->config(conf))
			{
				// TODO: Error during configuration
			}
		}
	}
}
