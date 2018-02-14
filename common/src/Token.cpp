#include "Token.hpp"

namespace zia::http
{
	const Token Token::eof = Token(TokenType::END_OF_FILE);
	const Token Token::unknown = Token(TokenType::UNKNOWN);

	Token::Token(TokenType t, std::string_view val) :
		type(t),
		value(val)
	{
	}
}