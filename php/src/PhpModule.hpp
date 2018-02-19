#pragma once

#include "api/module.h"
#include "Uri.hpp"

namespace zia::php
{
	class PhpModule final : public api::Module
	{
	public:
		PhpModule() = default;
		virtual ~PhpModule() = default;

		PhpModule(PhpModule const &) = default;
		PhpModule &operator=(PhpModule const &) = default;

		PhpModule(PhpModule &&) noexcept = default;
		PhpModule &operator=(PhpModule &&) noexcept = default;

		bool config(api::Conf const &conf) override;
		bool exec(api::HttpDuplex &http) override;

	private:
		bool executeRequest(api::HttpResponse &res, http::Uri const &uri);
	};
}