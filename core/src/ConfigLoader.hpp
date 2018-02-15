#pragma once

#include <string>
#include "api/conf.h"
#include "Hash.hpp"

namespace zia::core
{
	class ConfigLoader
	{
	public:
		explicit ConfigLoader(std::string filename);
		~ConfigLoader();

		ConfigLoader(ConfigLoader const &);
		ConfigLoader(ConfigLoader &&);
		ConfigLoader &operator=(ConfigLoader const &);
		ConfigLoader &operator=(ConfigLoader &&);

		void load();

		inline api::Conf const &getConfiguration() const noexcept
		{
			return m_config;
		}

		inline bool shouldReload() const noexcept
		{
			auto const &&currentHash = getHash();
			return m_hash != currentHash;
		}

	private:
		mutable crypto::Hash<crypto::AHash::MD5>	m_crypto;
		std::string	m_filename;
		zia::api::Conf	m_config;
		std::string	m_hash = "";

		std::string getHash() const noexcept;
	};
}
