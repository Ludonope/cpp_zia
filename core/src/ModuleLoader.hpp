#pragma once

#include "api/conf.h"

namespace zia::core
{
	class ModuleLoader
	{
	public:
		ModuleLoader(api::Conf const &conf);
		~ModuleLoader();

		ModuleLoader(ModuleLoader const &) = default;
		ModuleLoader(ModuleLoader &&) = default;
		ModuleLoader &operator=(ModuleLoader const &) = default;
		ModuleLoader &operator=(ModuleLoader &&) = default;

	private:
		api::Conf m_conf;
	};
}