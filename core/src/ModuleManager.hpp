#pragma once

#include <memory>
#include <vector>
#include "api/conf.h"
#include "api/net.h"
#include "api/module.h"

namespace zia::core
{
	class ModuleManager
	{
	public:
		ModuleManager(api::Conf const &conf);
		~ModuleManager();

		ModuleManager(ModuleManager const &) = default;
		ModuleManager(ModuleManager &&) = default;
		ModuleManager &operator=(ModuleManager const &) = default;
		ModuleManager &operator=(ModuleManager &&) = default;

		void start();
		void stop();

	private:
		using ModuleList = std::vector<std::unique_ptr<api::Module>>;
		api::Conf			m_conf;
		std::unique_ptr<api::Net>	m_networkModule = nullptr;
		ModuleList			m_receiveModule = {};
		ModuleList			m_processingModule = {};
		ModuleList			m_sendingModule = {};

		void	receiveCallback(api::Net::Raw &&raw, api::NetInfo &&infos);
		static void	configureModuleList(ModuleList &list, api::Conf const &conf);
	};
}