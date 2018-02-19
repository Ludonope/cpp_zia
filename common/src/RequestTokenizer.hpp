#pragma once

#include <string_view>
#include <vector>
#include "Token.hpp"

namespace zia::http
{
	class RequestTokenizer
	{
	public:
		RequestTokenizer() = delete;
		~RequestTokenizer() noexcept = default;

		RequestTokenizer(std::string_view input);

		RequestTokenizer(RequestTokenizer const &) = default;
		RequestTokenizer &operator=(RequestTokenizer const &) = default;

		RequestTokenizer(RequestTokenizer &&) noexcept = default;
		RequestTokenizer &operator=(RequestTokenizer &&) noexcept = default;

		auto const &getTokens() const
		{
			return m_tokens;
		}

	private:
		ReqToken parseId();
		ReqToken parsePunc();

		std::size_t m_cursor = 0;
		std::string_view m_input;
		std::vector<ReqToken> m_tokens = {};
	};
}