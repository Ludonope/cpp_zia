#pragma once

#include <string_view>
#include <vector>
#include <cstring>
#include "Token.hpp"

namespace zia::http
{
	class ConfigTokenizer
	{
	public:
		ConfigTokenizer() = delete;
		~ConfigTokenizer() noexcept = default;

		ConfigTokenizer(std::string_view input);

		ConfigTokenizer(ConfigTokenizer const &) = default;
		ConfigTokenizer &operator=(ConfigTokenizer const &) = default;

		ConfigTokenizer(ConfigTokenizer &&) noexcept = default;
		ConfigTokenizer &operator=(ConfigTokenizer &&) noexcept = default;

		auto const &getTokens() const
		{
			return m_tokens;
		}

	private:
		void skipSpaces();
		ConfigToken stringLiteral();
		ConfigToken number();
		ConfigToken punct();

		std::size_t m_cursor = 0;
		std::string_view m_input;
		std::vector<ConfigToken> m_tokens = {};
	};
}