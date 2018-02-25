#pragma once

#include <vector>
#include <string>
#include "api/module.h"
#include "Uri.hpp"

namespace zia
{
	class CustomRoot final : public api::Module
	{
	public:
		CustomRoot() = default;
		virtual ~CustomRoot() = default;

		CustomRoot(CustomRoot const &) = default;
		CustomRoot &operator=(CustomRoot const &) = default;

		CustomRoot(CustomRoot &&) noexcept = default;
		CustomRoot &operator=(CustomRoot &&) noexcept = default;

		bool config(api::Conf const &conf) override;
		bool exec(api::HttpDuplex &http) override;

	private:
		bool validatePath(std::string_view path, api::HttpResponse &res) const;

		std::string m_root;
	};
}
