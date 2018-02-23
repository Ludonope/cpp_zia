#include <sstream>
#include <cctype>
#include "Uri.hpp"
#include "Lexer.hpp"

namespace zia::http
{
	Uri::Uri(std::string_view uri)
	{
		this->set(uri);
	}

	Uri &Uri::operator=(std::string_view uri)
	{
		this->set(uri);

		return *this;
	}

	void Uri::set(std::string_view uri)
	{
		UriLexer lex(uri);

		this->clear();

		if (lex.peekIs({ UriTokenType::COLON }, 1) || lex.peekIs({ UriTokenType::COLON }))
		{
			if (lex.peekIs({ UriTokenType::ID }))
			{
				m_scheme = lex.next().value;
			}

			if (lex.peekIs({ UriTokenType::SLASH }) && lex.peekIs({ UriTokenType::SLASH }, 1))
			{
				this->parseHostGroup(lex);
			}

		}

		if (lex.peekIs({ UriTokenType::SLASH }))
		{
			lex.next();
			while (lex && lex.peekIs({ UriTokenType::QUESTION, UriTokenType::HASH }) == false)
			{
				auto next = lex.next();

				m_path += next.value;
			}
		}

		if (lex.peekIs({ UriTokenType::QUESTION }))
		{
			lex.next();
			m_query = lex.nextChecked(UriTokenType::ID, "Expected a query after the question mark").value;
		}

		if (lex.peekIs({ UriTokenType::HASH }))
		{
			lex.next();
			m_query = lex.nextChecked(UriTokenType::ID, "Expected a fragment after the hash").value;
		}

		lex.nextChecked(UriTokenType::END_OF_FILE, "Unexpected content at the end of the uri");

		this->normalizeAll();
	}

	static std::uint16_t defaultSchemePort(std::string_view scheme)
	{
		if (scheme == "http")
		{
			return 80;
		}
		else if (scheme == "https")
		{
			return 443;
		}
		else
		{
			return 0;
		}
	}

	std::string Uri::toString() const
	{
		std::ostringstream ss;

		ss << m_scheme << ':';

		if (m_host != "")
		{
			ss << "//";

			if (m_user != "")
			{
				ss << m_user;

				if (m_password != "")
				{
					ss << ':' << m_password;
				}

				ss << '@';
			}

			ss << m_host;

			if (m_port != defaultSchemePort(m_scheme))
			{
				ss << ':' << m_port;
			}
		}

		if (m_path != "")
		{
			ss << '/' << m_path;
		}

		if (m_query != "")
		{
			ss << '?' << m_query;
		}

		if (m_fragment != "")
		{
			ss << '#' << m_fragment;
		}

		return ss.str();
	}

	std::string Uri::decode(std::string_view s)
	{
		auto str = std::string();
		auto val = std::string();
		auto percent = 0;

		str.reserve(s.length());
		val.reserve(3);

		for (char c : s)
		{
			if (percent > 1)
			{
				val += c;
				--percent;
			}
			else if (percent == 1)
			{
				str += static_cast<char>(std::stoi(val, nullptr, 16));
				--percent;
			}
			else if (c == '%')
			{
				percent = 3;
			}
			else
			{
				str += c;
			}
		}

		return str;
	}

	void Uri::parseHostGroup(UriLexer &lex)
	{
		lex.next();
		lex.next();

		if (lex.peekIs({ UriTokenType::AT }, 1) || lex.peekIs({ UriTokenType::AT }, 3))
		{
			m_user = lex.nextChecked(UriTokenType::ID, "Expected a user name").value;
			if (lex.peekIs({ UriTokenType::COLON }))
			{
				lex.next();
				m_password = lex.nextChecked(UriTokenType::ID, "Expected a password").value;
			}
			lex.next();
		}
		m_host = lex.nextChecked(UriTokenType::ID, "Expected a host name").value;
		
		if (lex.peekIs({ UriTokenType::COLON }))
		{
			lex.next();
			m_port = std::stoi(std::string(lex.next().value));
		}
	}

	void Uri::normalizeAll()
	{
		normalize(m_scheme);
		normalize(m_user);
		normalize(m_password);
		normalize(m_host);
		normalize(m_path, true);
		normalize(m_query, true);
		normalize(m_fragment, true);

		decodeValue(m_scheme);
		decodeValue(m_user);
		decodeValue(m_password);
		decodeValue(m_host);
		decodeValue(m_path);
		decodeValue(m_fragment);
	}

	void Uri::normalize(std::string &e, bool percentOnly)
	{
		int pct = 0;

		for (char &c : e)
		{
			if (c == '%')
			{
				pct = 2;
			}

			if (pct)
			{
				c = std::toupper(c);
				--pct;
			}
			else if (!percentOnly)
			{
				c = std::tolower(c);
			}
		}
	}

	void Uri::clear()
	{
		m_scheme.clear();
		m_user.clear();
		m_password.clear();
		m_host.clear();
		m_port = 0;
		m_path.clear();
		m_query.clear();
		m_fragment.clear();
	}

	void Uri::decodeValue(std::string &s)
	{
		s = decode(s);
	}
}