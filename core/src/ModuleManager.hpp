#pragma once

#include <memory>
#include <vector>
#include <string>
#include "api/conf.h"
#include "api/net.h"
#include "api/module.h"
#include "LibLoader.hpp"

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
		using ModuleDef = std::pair<std::unique_ptr<api::Module>, std::string>;
		using ModuleList = std::vector<ModuleDef>;
		using LibraryList = std::vector<LibLoader>;
		api::Conf			m_conf;
		LibraryList			m_libs = {};
		std::unique_ptr<api::Net>	m_networkModule = nullptr;
		ModuleList			m_receiveModule = {};
		ModuleList			m_processingModule = {};
		ModuleList			m_sendingModule = {};

		void	loadModules() noexcept;
		using ModuleConfList = std::vector<std::string>;
		ModuleConfList getModulesPath() const noexcept;
		using ModuleNameList = std::tuple<std::string, ModuleConfList, ModuleConfList, ModuleConfList>;
		ModuleNameList getModules() const noexcept;
		ModuleConfList getModuleList(api::ConfArray const &) const noexcept;

		void	receiveCallback(api::Net::Raw const &raw, api::NetInfo const &infos);
		static void	configureModuleList(ModuleList &list, api::Conf const &conf);

#if defined __linux__
		static constexpr auto ModulePrefix = "lib";
		static constexpr auto ModuleExtension = ".so";
#elif defined __APPLE__
		static constexpr auto ModulePrefix = "lib";
		static constexpr auto ModuleExtension = ".dylib";
#elif defined _WIN32
		static constexpr auto ModulePrefix = "";
		static constexpr auto ModuleExtension = ".dll";
#else
#error	"Operating system not supported"
#endif
	};
}