#include <array>
#include <utility>
#include <cctype>
#include "ConfigTokenizer.hpp"

namespace zia::http
{
	ConfigTokenizer::ConfigTokenizer(std::string_view input) :
		m_input(input)
	{
		auto size = m_input.size();

		while (m_cursor < size)
		{
			auto c = m_input[m_cursor];

			if (std::isspace(c))
			{
				this->skipSpaces();
			}
			else if (c == '"')
			{
				m_tokens.push_back(this->stringLiteral());
			}
			else if (std::isdigit(c) || c == '.')
			{
				m_tokens.push_back(this->number());
			}
			else
			{
				m_tokens.push_back(this->punct());
			}
		}
	}

	void ConfigTokenizer::skipSpaces()
	{
		auto size = m_input.size();

		while (m_cursor < size && std::isspace(m_input[m_cursor]))
		{
			++m_cursor;
		}
	}

	ConfigToken ConfigTokenizer::stringLiteral()
	{
		auto begin = m_cursor + 1;
		auto esc = false;
		auto size = m_input.size();

		++m_cursor;

		while (m_cursor < size)
		{
			auto c = m_input[m_cursor];

			if (c == '"' && esc == false)
			{
				break;
			}
			esc = (c == '\\' && !esc);

			++m_cursor;
		}

		if (m_cursor == size)
		{
			throw std::invalid_argument("Cannot find a closing '\"'");
		}

		auto sub = m_input.substr(begin, m_cursor - begin);
		
		++m_cursor;

		return ConfigToken(ConfigTokenType::STRING_LITERAL, sub);
	}

	ConfigToken ConfigTokenizer::number()
	{
		auto begin = m_cursor;
		auto size = m_input.size();
		auto type = ConfigTokenType::NUMBER;

		while (m_cursor < size && std::isdigit(m_input[m_cursor]))
		{
			++m_cursor;
		}

		if (m_cursor < size && m_input[m_cursor] == '.')
		{
			type = ConfigTokenType::DOUBLE;
			++m_cursor;
			while (m_cursor < size && std::isdigit(m_input[m_cursor]))
			{
				++m_cursor;
			}
		}

		auto sub = m_input.substr(begin, m_cursor - begin);

		return ConfigToken(type, sub);
	}

	ConfigToken ConfigTokenizer::punct()
	{
		static const std::array<std::pair<char const *, ConfigTokenType>, 8> punc = {{
			{ "{", ConfigTokenType::OPEN_BRACE },
			{ "}", ConfigTokenType::CLOSE_BRACE },
			{ "[", ConfigTokenType::OPEN_BRACKET },
			{ "]", ConfigTokenType::CLOSE_BRACKET },
			{ ",", ConfigTokenType::COMMA },
			{ ":", ConfigTokenType::COLON },
			{ "true", ConfigTokenType::BOOLEAN },
			{ "false", ConfigTokenType::BOOLEAN }
		}};

		for (auto[val, token] : punc)
		{
			auto len = std::strlen(val);
			auto sub = m_input.substr(m_cursor, len);

			if (std::strncmp(val, sub.data(), len) == 0)
			{
				m_cursor += len;
				return ConfigToken(token, sub);
			}
		}

		throw std::invalid_argument("Unknown token");
	}
}