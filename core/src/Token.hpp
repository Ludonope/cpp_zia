#pragma once

#include <string_view>
#include <vector>

namespace zia::http
{
	enum class TokenType
	{
		END_OF_FILE,
		CRLF,
		SPACE,
		ID,
		EXCLAMATION,
		DOLLAR,
		AMPERSAND,
		APOSTROPHE,
		OPEN_PAREN,
		CLOSE_PAREN,
		ASTERISK,
		PLUS,
		COMMA,
		SEMICOLON,
		EQUAL,
		PERCENT,
		COLON,
		AT,
		QUESTION,
		SLASH,
		DOT,
		UNKNOWN
	};

	struct Token
	{
		Token() = delete;
		~Token() noexcept = default;

		Token(TokenType t, std::string_view val = "");

		Token(Token const &) = default;
		Token &operator=(Token const &) = default;

		Token(Token &&) noexcept = default;
		Token &operator=(Token &&) noexcept = default;

		TokenType type;
		std::string_view value;
		std::vector<Token> childs = {};

		static const Token eof;
		static const Token unknown;
	};
}