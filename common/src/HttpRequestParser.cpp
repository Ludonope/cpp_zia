#include <array>
#include <utility>
#include <string>
#include <cstring>
#include "HttpRequestParser.hpp"
#include "Lexer.hpp"

namespace zia::http
{
	static api::http::Method parseMethod(RequestLexer &lex)
	{
		auto meth = lex.nextChecked(RequestTokenType::ID, "Expected an identifier as a method");
		static const std::array<std::pair<const char *, api::http::Method>, 8> val{ {
			{ "OPTIONS", api::http::Method::options },
			{ "GET", api::http::Method::get },
			{ "HEAD", api::http::Method::head },
			{ "POST", api::http::Method::post },
			{ "PUT", api::http::Method::put },
			{ "DELETE", api::http::Method::delete_ },
			{ "TRACE", api::http::Method::trace },
			{ "CONNECT", api::http::Method::connect }
		}};

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

		while (lex && !lex.peekIs({ RequestTokenType::CRLF, RequestTokenType::SPACE }))
		{
			auto next = lex.next();

			if (next.type == RequestTokenType::UNKNOWN)
			{
				throw std::invalid_argument("Invalid token in uri");
			}

			uri += lex.next().value;
		}

		// TODO: decode the uri (ex: "yolo.com/s?hello%20world" -> "yolo.com/s?hello world")

		return uri;
	}

	static api::http::Version parseHttpVersion(RequestLexer &lex)
	{
		std::string version;

		while (lex && !lex.peekIs({ RequestTokenType::CRLF, RequestTokenType::SPACE }))
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

		while (peek != RequestTokenType::END_OF_FILE &&
			peek != RequestTokenType::COLON)
		{
			header += lex.next().value;
			peek = lex.peek().type;
		}

		lex.nextChecked(RequestTokenType::COLON, "Expected a ':' between the header name's and the content");

		std::string content;
		peek = lex.peek().type;

		while (peek != RequestTokenType::END_OF_FILE &&
			peek != RequestTokenType::CRLF)
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
		lex.nextChecked(RequestTokenType::SPACE, "Expected a space after the method");

		req.uri = parseUri(lex);
		lex.nextChecked(RequestTokenType::SPACE, "Expected a space after the uri");

		req.version = parseHttpVersion(lex);
		lex.nextChecked(RequestTokenType::CRLF, "Expected a CRLF after the http version");

		while (lex && lex.peek().type != RequestTokenType::CRLF)
		{
			auto[header, content] = parseHeader(lex);

			req.headers[header] = content;
			lex.nextChecked(RequestTokenType::CRLF, "Expected a CRLF after a header");
		}
		lex.nextChecked(RequestTokenType::CRLF, "Expected a CRLF at the end of the request");

		return req;
	}

	api::HttpRequest parseRequest(std::string_view input, std::size_t &bodyLen)
	{
		auto req = parseRequest(input);
		auto contentLength = req.headers.find("Content-Length");

		if (contentLength != req.headers.end())
		{
			bodyLen = std::stol(contentLength->second);
		}
		else
		{
			bodyLen = 0;
		}

		return req;
	}

	static std::string_view getHeader(std::byte const *raw)
	{
		auto input = reinterpret_cast<char const *>(raw);
		std::size_t size = 0;

		// Loop until we hit the double CRLF
		while (std::strncmp(&input[size], "\r\n\r\n", 4))
		{
			++size;
		}

		size += 4;

		return std::string_view(input, size);
	}

	api::HttpRequest parseRequest(std::byte const *raw)
	{
		return parseRequest(getHeader(raw));
	}

	api::HttpRequest parseRequest(std::byte const *raw, std::size_t &bodyLen)
	{
		return parseRequest(getHeader(raw), bodyLen);
	}
}