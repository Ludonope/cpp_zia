#include <fstream>
#include <sstream>
#include "ConfigLoader.hpp"

namespace zia::core
{
	ConfigLoader::ConfigLoader(std::string filename) :
		m_crypto(),
		m_filename(std::move(filename))
	{
	}

	ConfigLoader::ConfigLoader(ConfigLoader const &other) :
		m_config(other.m_config)
	{
	}

	ConfigLoader::ConfigLoader(ConfigLoader &&other) :
		m_config(std::move(other.m_config))
	{
	}

	ConfigLoader &ConfigLoader::operator=(ConfigLoader const &other)
	{
		if (this != &other)
		{
			m_config = other.m_config;
		}
		return *this;
	}

	ConfigLoader &ConfigLoader::operator=(ConfigLoader &&other)
	{
		if (this != &other)
		{
			m_config = std::move(other.m_config);
		}
		return *this;
	}

	void ConfigLoader::load()
	{
#if 1
		// Mock datas
		api::ConfArray const modulesPath = {
			api::ConfValue{std::string{"."}},
			api::ConfValue{std::string{"./libs"}},
			api::ConfValue{std::string{"./modules"}}
		};
		api::ConfObject modulesLists = {};
		modulesLists["network"] = api::ConfValue{std::string{"zia_network"}};
		modulesLists["receive"] = api::ConfValue{api::ConfArray{}};
		modulesLists["processing"] = api::ConfValue{api::ConfArray{api::ConfValue{std::string{"zia_php"}}}};
		modulesLists["send"] = api::ConfValue{api::ConfArray{}};

		api::ConfObject networkConfiguration = {};
		networkConfiguration["port"] = api::ConfValue{8080ll};
		networkConfiguration["cert"] = api::ConfValue{std::string{"./zia.crt"}};
		networkConfiguration["key"] = api::ConfValue{std::string{"./zia.key"}};

		m_config = api::ConfObject{};
		m_config["modules_path"] = api::ConfValue{modulesPath};
		m_config["modules"] = api::ConfValue{modulesLists};
		m_config["network"] = api::ConfValue{networkConfiguration};
#endif
		// TODO: Implement and fill m_config
		m_hash = getHash();
	}

	std::string ConfigLoader::getHash() const noexcept
	{
		std::ifstream file(m_filename.c_str());
		if (!file.good() && !file.is_open()) {
			return "";
		}
		std::stringstream buffer;

		buffer << file.rdbuf();
		m_crypto.compute(reinterpret_cast<std::uint8_t const *>(buffer.str().c_str()),
			sizeof(buffer.str().length()));
		return std::string(m_crypto.getHash());
	}
}
