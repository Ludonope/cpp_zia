#include <array>
#include <utility>
#include "UriTokenizer.hpp"

namespace zia::http
{
	void UriTokenizer::add(UriTokenType type)
	{
		m_tokens.emplace_back(type, m_input.substr(m_cursor, 1));
	}

	UriTokenizer::UriTokenizer(std::string_view input) :
		m_input(input)
	{
		std::size_t begin = 0;
		UriTokenType toAdd;

		while (m_cursor < input.size())
		{
			bool symbol = true;

			switch (input[m_cursor])
			{
			case ':':
				toAdd = UriTokenType::COLON;
				break;
			case '/':
				toAdd = UriTokenType::SLASH;
				break;
			case '@':
				toAdd = UriTokenType::AT;
				break;
			case '?':
				toAdd = UriTokenType::QUESTION;
				break;
			case '#':
				toAdd = UriTokenType::HASH;
				break;
			default:
				symbol = false;
				break;
			}

			if (symbol == true)
			{
				if (begin != m_cursor)
				{
					m_tokens.emplace_back(UriTokenType::ID, m_input.substr(begin, m_cursor - begin));
				}
				begin = m_cursor + 1;
				this->add(toAdd);
			}

			m_cursor++;
		}
		if (begin != m_cursor)
		{
			m_tokens.emplace_back(UriTokenType::ID, m_input.substr(begin, m_cursor - begin));
		}
	}
}