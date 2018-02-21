#pragma once

#include <vector>
#include "Token.hpp"
#include "RequestTokenizer.hpp"
#include "UriTokenizer.hpp"
#include "ConfigTokenizer.hpp"

namespace zia::http
{
	template <class Tokenizer, class Token>
	class Lexer
	{
	public:
		Lexer() = default;
		~Lexer() noexcept = default;

		Lexer(std::string_view input)
		{
			this->lex(input);
		}

		Lexer(Lexer const &) = default;
		Lexer &operator=(Lexer const &) = default;

		Lexer(Lexer &&) noexcept = default;
		Lexer &operator=(Lexer &&) noexcept = default;

		void lex(std::string_view input)
		{
			m_tokens = Tokenizer(input).getTokens();
		}

		Token peek(std::size_t n = 0) const
		{
			if (m_cur + n < m_tokens.size())
			{
				return m_tokens[m_cur + n];
			}
			else
			{
				return Token(Token::Type::END_OF_FILE);
			}
		}

		bool peekIs(std::initializer_list<typename Token::Type> list, std::size_t n = 0)
		{
			auto peek = this->peek(n);

			for (auto type : list)
			{
				if (peek.type == type)
				{
					return true;
				}
			}

			return false;
		}

		Token next()
		{
			if (m_cur < m_tokens.size())
			{
				return m_tokens[m_cur++];
			}
			else
			{
				return Token(Token::Type::END_OF_FILE);
			}
		}

		Token nextChecked(typename Token::Type t, std::string msg)
		{
			return this->nextChecked({ t }, std::move(msg));
		}

		Token nextChecked(std::initializer_list<typename Token::Type> t, std::string msg)
		{
			auto token = this->next();

			for (auto type : t)
			{
				if (token.type == type)
				{
					return token;
				}
			}

			throw std::invalid_argument(msg);
		}

		operator bool()
		{
			return m_cur < m_tokens.size();
		}

	private:
		std::vector<Token> m_tokens;
		std::size_t m_cur = 0;
	};

	using RequestLexer = Lexer<RequestTokenizer, ReqToken>;
	using UriLexer = Lexer<UriTokenizer, UriToken>;
	using ConfLexer = Lexer<ConfigTokenizer, ConfigToken>;
}