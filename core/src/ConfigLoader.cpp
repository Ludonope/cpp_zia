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
		// TODO: Implement and fill m_config
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
