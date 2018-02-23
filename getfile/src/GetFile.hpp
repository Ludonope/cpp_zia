#pragma once

#include "api/module.h"
#include "Uri.hpp"

namespace zia
{
	class GetFile final : public api::Module
	{
	public:
		GetFile() = default;
		virtual ~GetFile() = default;

		GetFile(GetFile const &) = default;
		GetFile &operator=(GetFile const &) = default;

		GetFile(GetFile &&) noexcept = default;
		GetFile &operator=(GetFile &&) noexcept = default;

		bool config(api::Conf const &conf) override;
		bool exec(api::HttpDuplex &http) override;
	};
}