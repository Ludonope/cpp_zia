#include "ConfigParser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"

namespace zia::conf
{
	static api::ConfValue parseConfValue(http::ConfLexer &lex);

	static std::string destringify(std::string_view s)
	{
		auto str = std::string();
		bool esc = false;

		str.reserve(str.size());

		for (auto c : s)
		{
			if (esc)
			{
				switch (c)
				{
					case '\'':
					case '"':
					case '\\':
						str += c;
						break;
					case 'n':
						str += '\n';
						break;
					case 'r':
						str += '\r';
						break;
					case 't':
						str += '\t';
						break;
					case 'b':
						str += '\b';
						break;
					case 'f':
						str += '\f';
						break;
					case 'v':
						str += '\v';
						break;
					case '0':
						str += '\0';
						break;
				}
			}
			else
			{
				str += c;
			}
			esc = (c == '\\' && esc == false);
		}

		return str;
	}

	static bool parseConfBoolean(http::ConfLexer &lex)
	{
		return (lex.next().value == "true");
	}

	static double parseConfDouble(http::ConfLexer &lex)
	{
		return std::stod(std::string(lex.next().value));
	}

	static long long parseConfNumber(http::ConfLexer &lex)
	{
		return std::stoll(std::string(lex.next().value));
	}

	static std::string parseConfString(http::ConfLexer &lex)
	{
		return destringify(lex.next().value);
	}

	static api::ConfArray parseConfArray(http::ConfLexer &lex)
	{
		auto arr = api::ConfArray();
		bool first = true;

		lex.nextChecked(ConfigTokenType::OPEN_BRACKET, "Expected a '['");

		while (lex && lex.peekIs({ ConfigTokenType::CLOSE_BRACKET }) == false)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				lex.nextChecked(ConfigTokenType::COMMA, "Expected a ',' array elements");
			}

			arr.push_back(parseConfValue(lex));
		}

		lex.nextChecked(ConfigTokenType::CLOSE_BRACKET, "Expected a ']'");

		return arr;
	}

	static api::ConfObject parseConfObject(http::ConfLexer &lex)
	{
		auto obj = api::ConfObject();
		bool first = true;
		
		lex.nextChecked(ConfigTokenType::OPEN_BRACE, "Expected a '{'");

		while (lex && lex.peekIs({ ConfigTokenType::CLOSE_BRACE }) == false)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				lex.nextChecked(ConfigTokenType::COMMA, "Expected a ',' between properties");
			}

			auto nameToken = lex.nextChecked(ConfigTokenType::STRING_LITERAL, "Expected a property name");
			auto name = destringify(nameToken.value);

			lex.nextChecked(ConfigTokenType::COLON, "Expected a ':' after a property name");

			obj[name] = parseConfValue(lex);
		}

		lex.nextChecked(ConfigTokenType::CLOSE_BRACE, "Expected a '}'");

		return obj;
	}

	static api::ConfValue parseConfValue(http::ConfLexer &lex)
	{
		switch (lex.peek().type)
		{
		case ConfigTokenType::OPEN_BRACE:
			return { parseConfObject(lex) };
		case ConfigTokenType::OPEN_BRACKET:
			return { parseConfArray(lex) };
		case ConfigTokenType::STRING_LITERAL:
			return { parseConfString(lex) };
		case ConfigTokenType::NUMBER:
			return { parseConfNumber(lex) };
		case ConfigTokenType::DOUBLE:
			return { parseConfDouble(lex) };
		case ConfigTokenType::BOOLEAN:
			return { parseConfBoolean(lex) };
		default:
			throw std::invalid_argument("Unexpected token in configuration");
		}
	}

	api::Conf parseConf(std::string_view input)
	{
		auto lex = http::ConfLexer(input);

		return parseConfObject(lex);
	}
}