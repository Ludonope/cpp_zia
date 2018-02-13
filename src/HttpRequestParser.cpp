#include <array>
#include <utility>
#include "HttpRequestParser.hpp"
#include "RequestLexer.hpp"

namespace zia::http
{
	static api::http::Method parseMethod(RequestLexer &lex)
	{
		auto meth = lex.nextChecked(TokenType::ID, "Expected an identifier as a method");
		static const std::array<std::pair<const char *, api::http::Method>, 8> val{ {
			{ "OPTIONS", api::http::Method::options },
		{ "GET", api::http::Method::get },
		{ "HEAD", api::http::Method::head },
		{ "POST", api::http::Method::post },
		{ "PUT", api::http::Method::put },
		{ "DELETE", api::http::Method::delete_ },
		{ "TRACE", api::http::Method::trace },
		{ "CONNECT", api::http::Method::connect }
			} };

		for (auto const &[text, value] : val)
		{
			if (meth.value == text)
			{
				return value;
			}
		}

		return api::http::Method::unknown;
	}

	static std::string parseUri(RequestLexer &lex)
	{
		std::string uri;

		while (lex && !lex.peekIs({ TokenType::CRLF, TokenType::SPACE }))
		{
			uri += lex.next().value;
		}

		return uri;
	}

	static api::http::Version parseHttpVersion(RequestLexer &lex)
	{
		std::string version;

		while (lex && !lex.peekIs({ TokenType::CRLF, TokenType::SPACE }))
		{
			version += lex.next().value;
		}

		static const std::array<std::pair<const char *, api::http::Version>, 4> vals{{
			{ "HTTP/0.9", api::http::Version::http_0_9 },
			{ "HTTP/1.0", api::http::Version::http_1_0 },
			{ "HTTP/1.1", api::http::Version::http_1_1 },
			{ "HTTP/2.0", api::http::Version::http_2_0 }
		}};

		for (auto const &[text, ver] : vals)
		{
			if (version == text)
			{
				return ver;
			}
		}

		return api::http::Version::unknown;
	}

	static std::pair<std::string, std::string> parseHeader(RequestLexer &lex)
	{
		std::string header;
		auto peek = lex.peek().type;

		while (peek != TokenType::END_OF_FILE &&
			peek != TokenType::COLON)
		{
			header += lex.next().value;
			peek = lex.peek().type;
		}

		lex.nextChecked(TokenType::COLON, "Expected a ':' between the header name's and the content");

		std::string content;
		peek = lex.peek().type;

		while (peek != TokenType::END_OF_FILE &&
			peek != TokenType::CRLF)
		{
			content += lex.next().value;
			peek = lex.peek().type;
		}

		return { header, content };
	}

	api::HttpRequest parseRequest(std::string_view input)
	{
		RequestLexer lex(input);
		api::HttpRequest req;

		req.method = parseMethod(lex);
		lex.nextChecked(TokenType::SPACE, "Expected a space after the method");

		req.uri = parseUri(lex);
		lex.nextChecked(TokenType::SPACE, "Expected a space after the uri");

		req.version = parseHttpVersion(lex);
		lex.nextChecked(TokenType::CRLF, "Expected a CRLF after the http version");

		while (lex && lex.peek().type != TokenType::CRLF)
		{
			auto[header, content] = parseHeader(lex);

			req.headers[header] = content;
			lex.nextChecked(TokenType::CRLF, "Expected a CRLF after a header");
		}
		lex.nextChecked(TokenType::CRLF, "Expected a CRLF at the end of the request");

		return req;
	}
}