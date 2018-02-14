#pragma once

#include <string_view>
#include <vector>
#include "Token.hpp"

namespace zia::http
{
	class Tokenizer
	{
	public:
		Tokenizer() = delete;
		~Tokenizer() noexcept = default;

		Tokenizer(std::string_view input);

		Tokenizer(Tokenizer const &) = default;
		Tokenizer &operator=(Tokenizer const &) = default;

		Tokenizer(Tokenizer &&) noexcept = default;
		Tokenizer &operator=(Tokenizer &&) noexcept = default;

		auto const &getTokens() const
		{
			return m_tokens;
		}

	private:
		Token parseId();
		Token parsePunc();

		std::size_t m_cursor = 0;
		std::string_view m_input;
		std::vector<Token> m_tokens = {};
	};
}