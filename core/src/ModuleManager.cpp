#include <iostream>
#include <variant>
#include "ModuleManager.hpp"
#include "HttpRequestParser.hpp"
#include "FileHelper.hpp"
#include "ResponseSerializer.hpp"

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
#if _WIN32
								m_correspondanceTable[moduleFullPath] = module.substr(0, module.length() - 4);
#else
								m_correspondanceTable[moduleFullPath] = module.substr(3, module.length() - 6);
#endif
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
			try
			{
				if (!m_networkModule->config(std::get<api::ConfObject>(m_conf.at("network").v)))
				{
					std::cerr << "Error loading network "
						"module configuration" << std::endl;
				}
			}
			catch (std::exception const &)
			{
				m_networkModule->config({});
				std::cerr << "Invalid configuration, loading network using default configuration" << std::endl;
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

	void ModuleManager::receiveCallback(api::Net::Raw &&raw, api::NetInfo &&infos)
	{
		api::HttpDuplex	duplex = {};

		duplex.info = std::move(infos);
		duplex.raw_req = std::move(raw);

		try
		{
			duplex.req = http::parseRequest(duplex.raw_req);
		}
		catch (std::exception const &e)
		{
			std::cout << "Error: ";
			std::cerr << e.what() << '\n' << std::endl;
			duplex.resp.version = api::http::Version::http_1_1;
			duplex.resp.status = api::http::common_status::internal_server_error;
			duplex.resp.reason = "Internal-Server-Error";
		}
		auto const moduleProcess = [&](ModuleList &list, bool mustBreak = false) {
			auto done = false;

			for (auto &module : list)
			{
				try
				{
					done = module.first->exec(duplex) || done;
					if (mustBreak && done)
					{
						return true;
					}
				}
				catch (std::exception const &e)
				{
					std::cerr << "Error: " << e.what() << '\n';
				}
			}
			return done;
		};

		moduleProcess(m_receiveModule);
		if (!moduleProcess(m_processingModule, true))
		{
			duplex.resp.version = api::http::Version::http_1_1;
			duplex.resp.status = api::http::common_status::not_implemented;
			duplex.resp.reason = "Not-Implemented";
		}
		moduleProcess(m_sendingModule);
	
		duplex.raw_resp = http::responseToRaw(duplex.resp);
		m_networkModule->send(duplex.info.sock, duplex.raw_resp);
	}

	void ModuleManager::configureModuleList(ModuleList &list, api::Conf const &conf)
	{
		for (auto &module : list)
		{
			try
			{
				auto const configName = m_correspondanceTable.at(module.second);
				if (!module.first->config(std::get<api::ConfObject>(conf.at(configName).v)))
				{
					std::cerr << "Error loading " <<
						module.second << " module configuration" << std::endl;
				}
			}
			catch (std::exception const &e)
			{
				std::cerr << e.what() << std::endl;
				module.first->config({});
				std::cerr << "Invalid configuration, loading " << module.second <<
					" module using default configuration" << std::endl;
			}
		}
	}
}
