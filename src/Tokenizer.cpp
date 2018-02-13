#include <cctype>
#include <utility>
#include <array>
#include <exception>
#include "Tokenizer.hpp"

namespace zia::http
{
	Tokenizer::Tokenizer(std::string_view input) : m_input(input)
	{
		while (m_cursor < m_input.size())
		{
			char c = m_input[m_cursor];

			if (std::isalpha(c) || c == '_')
			{
				this->parseId();
			}
			else
			{
				this->parsePunc();
			}
		}
	}

	Token Tokenizer::parseId()
	{
		auto begin = m_cursor;
		char c;

		--m_cursor;
		do
		{
			++m_cursor;
			c = m_input[begin];
		} while (std::isalnum(c) || c == '_');

		return Token(TokenType::ID, m_input.substr(begin, m_cursor - begin));
	}

	Token Tokenizer::parsePunc()
	{
		auto begin = m_cursor;
		static constexpr std::array<std::pair<const char *, TokenType>, 19> data{{
			{ "\r\n", TokenType::CRLF },
			{ " ", TokenType::SPACE },
			{ "!", TokenType::EXCLAMATION },
			{ "$", TokenType::DOLLAR },
			{ "&", TokenType::AMPERSAND },
			{ "'", TokenType::APOSTROPHE },
			{ "(", TokenType::OPEN_PAREN },
			{ ")", TokenType::CLOSE_PAREN },
			{ "*", TokenType::ASTERISK },
			{ "+", TokenType::PLUS },
			{ ",", TokenType::COMMA },
			{ ";", TokenType::SEMICOLON },
			{ "=", TokenType::EQUAL },
			{ "%", TokenType::PERCENT },
			{ ":", TokenType::COLON },
			{ "@", TokenType::AT },
			{ "?", TokenType::QUESTION },
			{ "/", TokenType::SLASH },
			{ ".", TokenType::DOT }
		}};

		for (auto const &val : data)
		{
			auto size = std::strlen(val.first);
			auto sub = m_input.substr(m_cursor, size);

			if (sub == val.first)
			{
				m_cursor += size;

				return Token(val.second, sub);
			}
		}

		auto sub = m_input.substr(m_cursor, 1);
		++m_cursor;

		return Token(TokenType::UNKNOWN, sub);
	}
}