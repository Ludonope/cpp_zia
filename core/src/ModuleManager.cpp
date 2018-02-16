#include <iostream>
#include <variant>
#include "ModuleManager.hpp"
#include "HttpRequestParser.hpp"

namespace zia::core
{
	ModuleManager::ModuleManager(api::Conf const &conf) :
		m_conf(conf)
	{
	}

	ModuleManager::~ModuleManager()
	{
	}

	ModuleManager::ModuleConfList ModuleManager::getModulesPath() const noexcept
	{
		std::vector<std::string>	modulePath = {};
		try
		{
			auto datas = m_conf.at("modules_path").v;
			auto array = std::get<api::ConfArray>(datas);
			for (auto const &elem : array)
			{
				modulePath.push_back(std::get<std::string>(elem.v));
			}
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
			modulePath = std::vector<std::string>{"./"};
		}
		return modulePath;
	}

	ModuleManager::ModuleConfList ModuleManager::getModuleList(api::ConfArray const &datas) const noexcept
	{
		try
		{
			std::vector<std::string>	modules = {};
			for (auto const &elem : datas)
			{
				auto const moduleName = std::get<std::string>(elem.v);
				modules.push_back(ModuleManager::ModulePrefix +
					moduleName + ModuleManager::ModuleExtension);
			}
			return modules;
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
		}
		return {};
	}

	ModuleManager::ModuleNameList ModuleManager::getModules() const noexcept
	{
		try
		{
			auto modules = m_conf.at("modules").v;
			auto datas = std::get<api::ConfObject>(modules);
			return {
				std::get<std::string>(datas.at("network").v),
				getModuleList(std::get<api::ConfArray>(datas.at("receive").v)),
				getModuleList(std::get<api::ConfArray>(datas.at("processing").v)),
				getModuleList(std::get<api::ConfArray>(datas.at("send").v))
			};
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
		}
		return {{}, {}, {}, {}};
	}

	void ModuleManager::loadModules() noexcept
	{
		auto const &&modulePath = std::move(getModulesPath());
		auto const [moduleNetwork, moduleReceive, moduleProcessing, moduleSend] = std::move(getModules());
		auto const moduleLoader = [&](auto moduleListInput, auto &moduleListOutput){
			for (auto const &path: modulePath)
			{
				for (auto const &module : moduleListInput) {
					auto moduleFullPath = path + '/' + module;
					// TODO: Check if exists and load

					std::cout << "Trying to load " << moduleFullPath << std::endl;
					moduleListOutput.push_back({nullptr, moduleFullPath});
				}
			}
		};
		moduleLoader(moduleReceive, m_receiveModule);
		moduleLoader(moduleProcessing, m_processingModule);
		moduleLoader(moduleSend, m_sendingModule);
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
				std::cerr << "Error loading network "
					"module configuration" << std::endl;
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
		duplex.req = http::parseRequest(raw);
		auto const moduleProcess = [&](ModuleList &list){
			for (auto &module : list)
			{
				module.first->exec(duplex);
			}
		};
		moduleProcess(m_receiveModule);
		moduleProcess(m_processingModule);
		moduleProcess(m_sendingModule);
		// TODO: duplex.raw_resp = http::toString(duplex.resp);
		duplex.raw_resp = api::Net::Raw{std::byte{'Y'}, std::byte{'a'}, std::byte{'y'}};
		m_networkModule->send(infos.sock, duplex.raw_resp);
	}

	void	ModuleManager::configureModuleList(ModuleList &list, api::Conf const &conf)
	{
		for (auto &module : list)
		{
			if (!module.first->config(conf))
			{
				std::cerr << "Error loading " <<
					module.second << " module configuration" << std::endl;
			}
		}
	}
}
