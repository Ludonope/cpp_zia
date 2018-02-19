#pragma once

#include <string>
#include "api/conf.h"
#include "Hash.hpp"
#include "Timer.hpp"

namespace zia::core
{
	class ConfigLoader
	{
	public:
		explicit ConfigLoader(std::string filename);
		~ConfigLoader() = default;

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
			if (m_timer.check())
			{
				auto const &&currentHash = getHash();
				return m_hash != currentHash;

			}
			return false;
		}

	private:
		mutable crypto::Hash<crypto::AHash::MD5>	m_crypto;
		mutable Timer<std::chrono::milliseconds>	m_timer = Timer<std::chrono::milliseconds>(500);
		std::string					m_filename;
		zia::api::Conf					m_config;
		std::string					m_hash = "";

		std::string getHash() const noexcept;
	};
}
