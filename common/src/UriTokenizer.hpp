#pragma once

#include <string_view>
#include <vector>
#include "Token.hpp"

namespace zia::http
{
	class UriTokenizer
	{
	public:
		UriTokenizer() = delete;
		~UriTokenizer() noexcept = default;

		UriTokenizer(std::string_view input);

		UriTokenizer(UriTokenizer const &) = default;
		UriTokenizer &operator=(UriTokenizer const &) = default;

		UriTokenizer(UriTokenizer &&) noexcept = default;
		UriTokenizer &operator=(UriTokenizer &&) noexcept = default;

		auto const &getTokens() const
		{
			return m_tokens;
		}

	private:
		void add(UriTokenType);

		std::size_t m_cursor = 0;
		std::string_view m_input;
		std::vector<UriToken> m_tokens = {};
	};
}