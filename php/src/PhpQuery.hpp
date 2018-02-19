#pragma once

#include <string_view>
#include <map>
#include <string>

namespace zia::php
{
	class PhpQuery : public std::map<std::string, std::string>
	{
	public:
		PhpQuery() = default;
		~PhpQuery() noexcept = default;

		PhpQuery(std::string_view query);

		PhpQuery(PhpQuery const &) = default;
		PhpQuery &operator=(PhpQuery const &) = default;

		PhpQuery(PhpQuery &&) = default;
		PhpQuery &operator=(PhpQuery &&) noexcept = default;
	};
}