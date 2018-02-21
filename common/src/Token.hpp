#pragma once

#include <string_view>
#include <vector>

namespace zia
{
	enum class RequestTokenType
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

	enum class UriTokenType
	{
		END_OF_FILE,
		COLON,
		SLASH,
		AT,
		QUESTION,
		HASH,
		ID
	};

	enum class ConfigTokenType
	{
		END_OF_FILE,
		OPEN_BRACE,
		CLOSE_BRACE,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		COLON,
		COMMA,
		STRING_LITERAL,
		NUMBER,
		DOUBLE,
		BOOLEAN,
		UNKNOWN
	};

	template <typename _Type>
	struct Token
	{
		Token() = delete;
		~Token() noexcept = default;

		Token(_Type t, std::string_view val = "") :
			type(t),
			value(val)
		{
		}

		Token(Token const &) = default;
		Token &operator=(Token const &) = default;

		Token(Token &&) noexcept = default;
		Token &operator=(Token &&) noexcept = default;

		_Type type;
		std::string_view value;
		std::vector<Token> childs = {};

		using Type = _Type;
	};

	using ReqToken = Token<RequestTokenType>;
	using UriToken = Token<UriTokenType>;
	using ConfigToken = Token<ConfigTokenType>;
}