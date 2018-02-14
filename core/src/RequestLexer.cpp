#include <exception>
#include "RequestLexer.hpp"
#include "Tokenizer.hpp"

namespace zia::http
{
	RequestLexer::RequestLexer(std::string_view input)
	{
		this->lex(input);
	}

	void RequestLexer::lex(std::string_view input)
	{
		m_tokens = Tokenizer(input).getTokens();
	}

	Token RequestLexer::peek(std::size_t n) const
	{
		if (m_cur + n < m_tokens.size())
		{
			return m_tokens[m_cur + n];
		}
		else
		{
			return Token::eof;
		}
	}

	bool RequestLexer::peekIs(std::initializer_list<TokenType> list, std::size_t n)
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

	Token RequestLexer::next()
	{
		if (m_cur < m_tokens.size())
		{
			return m_tokens[m_cur++];
		}
		else
		{
			return Token::eof;
		}
	}

	Token RequestLexer::nextChecked(TokenType t, std::string msg)
	{
		return this->nextChecked({ t }, std::move(msg));
	}

	Token RequestLexer::nextChecked(std::initializer_list<TokenType> t, std::string msg)
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

	RequestLexer::operator bool()
	{
		return m_cur < m_tokens.size();
	}
}