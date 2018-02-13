#pragma once

#include <vector>
#include "Token.hpp"

namespace zia::http
{
	class RequestLexer
	{
	public:
		RequestLexer() = default;
		~RequestLexer() noexcept = default;
		
		RequestLexer(std::string_view input);

		RequestLexer(RequestLexer const &) = default;
		RequestLexer &operator=(RequestLexer const &) = default;

		RequestLexer(RequestLexer &&) noexcept = default;
		RequestLexer &operator=(RequestLexer &&) noexcept = default;

		void lex(std::string_view input);

		Token peek(std::size_t n = 1) const;
		bool peekIs(std::initializer_list<TokenType> list, std::size_t n = 1);
		Token next();
		Token nextChecked(TokenType t, std::string msg);
		Token nextChecked(std::initializer_list<TokenType> t, std::string msg);

		operator bool();

	private:
		std::vector<Token> m_tokens;
		std::size_t m_cur = 0;
	};
}