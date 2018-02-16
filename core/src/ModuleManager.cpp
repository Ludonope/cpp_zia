#include <iostream>
#include <variant>
#include "ModuleManager.hpp"
#include "HttpRequestParser.hpp"
#include "FileHelper.hpp"

namespace zia::core
{
	ModuleManager::ModuleManager(api::Conf const &conf) :
		m_conf(conf)
	{
		loadModules();
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
				ModuleManager::ModulePrefix + std::get<std::string>(datas.at("network").v) + ModuleManager::ModuleExtension,
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
		auto const &&modulePath = getModulesPath();
		auto const [moduleNetwork, moduleReceive, moduleProcessing, moduleSend] = getModules();
		auto const moduleLoader = [&](auto moduleListInput, auto &moduleListOutput){
			for (auto const &path: modulePath)
			{
				for (auto const &module : moduleListInput) {
					auto moduleFullPath = path + '/' + module;
					if (zia::fs::exists(moduleFullPath))
					{
						try
						{
							m_libs.push_back(std::move(LibLoader(moduleFullPath)));
							auto const func = m_libs.back().getFunction<api::Module *()>("create");
							if (func)
							{
								moduleListOutput.push_back({std::unique_ptr<api::Module>(func()), moduleFullPath});
							}
						}
						catch (std::exception const &e)
						{
							std::cerr << e.what() << std::endl;
						}
					}
				}
			}
		};

		if (moduleNetwork != "")
		{
			for (auto const &path : modulePath)
			{
				auto const moduleFullPath = path + '/' + moduleNetwork;
				if (zia::fs::exists(moduleFullPath))
				{
					try
					{
						m_libs.push_back(std::move(LibLoader(moduleFullPath)));
						auto const func = m_libs.back().getFunction<api::Net *()>("create");
						if (func)
						{
							m_networkModule = std::unique_ptr<api::Net>(func());
						}
					}
					catch (std::exception const &e)
					{
						std::cout << e.what() << std::endl;
					}
					break;
				}
			}
		}
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

		try
		{
			std::cout << "Parsing request\n";
			duplex.req = http::parseRequest(raw);
			std::cout << "Request parsed\n";
			auto const moduleProcess = [&](ModuleList &list){
				for (auto &module : list)
				{
					module.first->exec(duplex);
				}
			};
			moduleProcess(m_receiveModule);
			moduleProcess(m_processingModule);
			moduleProcess(m_sendingModule);
			std::cout << "Execution over\n";
		}
		catch (std::exception const &e)
		{
			std::cout << "Error: ";
			std::cerr << e.what() << '\n' << std::endl;
			// TODO: Send error 500
		}
		// TODO: duplex.raw_resp = http::toString(duplex.resp);
		duplex.raw_resp = api::Net::Raw{std::byte{'Y'}, std::byte{'a'}, std::byte{'y'}};
		std::cout << "Sending response\n";
		m_networkModule->send(infos.sock, duplex.raw_resp);
		std::cout << "Done\n";
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
