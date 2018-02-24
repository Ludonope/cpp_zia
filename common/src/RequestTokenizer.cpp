#include <cctype>
#include <utility>
#include <array>
#include <exception>
#include <cstring>
#include "RequestTokenizer.hpp"

namespace zia::http
{
	RequestTokenizer::RequestTokenizer(std::string_view input) : m_input(input)
	{
		while (m_cursor < m_input.size())
		{
			char c = m_input[m_cursor];

			if (std::isalpha(c) || c == '_')
			{
				m_tokens.push_back(this->parseId());
			}
			else
			{
				m_tokens.push_back(this->parsePunc());
			}
		}
	}

	ReqToken RequestTokenizer::parseId()
	{
		auto begin = m_cursor;
		char c;

		--m_cursor;
		do
		{
			++m_cursor;
			c = m_input[m_cursor];
		} while (std::isalnum(c) || c == '_');

		return ReqToken(RequestTokenType::ID, m_input.substr(begin, m_cursor - begin));
	}

	ReqToken RequestTokenizer::parsePunc()
	{
		static constexpr std::array<std::pair<const char *, RequestTokenType>, 19> data{{
			{ "\r\n", RequestTokenType::CRLF },
			{ " ", RequestTokenType::SPACE },
			{ "!", RequestTokenType::EXCLAMATION },
			{ "$", RequestTokenType::DOLLAR },
			{ "&", RequestTokenType::AMPERSAND },
			{ "'", RequestTokenType::APOSTROPHE },
			{ "(", RequestTokenType::OPEN_PAREN },
			{ ")", RequestTokenType::CLOSE_PAREN },
			{ "*", RequestTokenType::ASTERISK },
			{ "+", RequestTokenType::PLUS },
			{ ",", RequestTokenType::COMMA },
			{ ";", RequestTokenType::SEMICOLON },
			{ "=", RequestTokenType::EQUAL },
			{ "%", RequestTokenType::PERCENT },
			{ ":", RequestTokenType::COLON },
			{ "@", RequestTokenType::AT },
			{ "?", RequestTokenType::QUESTION },
			{ "/", RequestTokenType::SLASH },
			{ ".", RequestTokenType::DOT }
		}};

		for (auto const &val : data)
		{
			auto size = std::strlen(val.first);
			auto sub = m_input.substr(m_cursor, size);

			if (sub == val.first)
			{
				m_cursor += size;

				return ReqToken(val.second, sub);
			}
		}

		auto sub = m_input.substr(m_cursor, 1);
		++m_cursor;

		return ReqToken(RequestTokenType::UNKNOWN, sub);
	}
}