#pragma once

#include <vector>
#include <string>
#include "api/module.h"
#include "Uri.hpp"

namespace zia
{
	class AutoIndex final : public api::Module
	{
	public:
		AutoIndex() = default;
		virtual ~AutoIndex() = default;

		AutoIndex(AutoIndex const &) = default;
		AutoIndex &operator=(AutoIndex const &) = default;

		AutoIndex(AutoIndex &&) noexcept = default;
		AutoIndex &operator=(AutoIndex &&) noexcept = default;

		bool config(api::Conf const &conf) override;
		bool exec(api::HttpDuplex &http) override;

	private:
		std::vector<std::string> m_defaultFiles = { };
	};
}