#if defined __clang__
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif

#include <string>
#include <cstring>
#include "gtest/gtest.h"
#include "HttpRequestParser.hpp"

using std::operator""s;

TEST(RequestParser, Valid1)
{
	auto input = "GET /path HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_1);
	ASSERT_EQ(req.method, zia::api::http::Method::get);
	ASSERT_EQ(req.uri, "/path");
	ASSERT_EQ(req.headers.size(), 0);
}

TEST(RequestParser, Valid2)
{
	auto input = "GET /path HTTP/1.1\r\n\r\n";
	auto contentSize = std::size_t(1);
	auto req = zia::http::parseRequest(input, contentSize);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_1);
	ASSERT_EQ(req.method, zia::api::http::Method::get);
	ASSERT_EQ(req.uri, "/path");
	ASSERT_EQ(req.headers.size(), 0);
	ASSERT_EQ(contentSize, 0);
}

TEST(RequestParser, WithContent1)
{
	auto const body = "hello world"s;
	auto const len = body.size();
	auto input = "GET / HTTP/1.1\r\nContent-Length: "s +
		std::to_string(len) + "\r\n\r\n" + body;
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_1);
	ASSERT_EQ(req.method, zia::api::http::Method::get);
	ASSERT_EQ(req.uri, "/");
	ASSERT_EQ(req.headers.size(), 1);
	ASSERT_EQ(std::stoll(req.headers.at("Content-Length")), len);
}

TEST(RequestParser, WithContent2)
{
	auto const body = "hello world"s;
	auto const len = body.size();
	auto input = "GET / HTTP/1.1\r\nContent-Length: "s +
		std::to_string(len) + "\r\n\r\n" + body;
	auto contentSize = std::size_t(0);
	auto req = zia::http::parseRequest(input, contentSize);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_1);
	ASSERT_EQ(req.method, zia::api::http::Method::get);
	ASSERT_EQ(req.uri, "/");
	ASSERT_EQ(req.headers.size(), 1);
	ASSERT_EQ(contentSize, len);
	ASSERT_EQ(std::stoll(req.headers.at("Content-Length")), len);
}

TEST(RequestParser, Method_Unknown)
{
	auto input = "YOLO / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::unknown);
}

TEST(RequestParser, Method_Options)
{
	auto input = "OPTIONS / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::options);
}

TEST(RequestParser, Method_Get)
{
	auto input = "GET / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::get);
}

TEST(RequestParser, Method_Head)
{
	auto input = "HEAD / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::head);
}

TEST(RequestParser, Method_Post)
{
	auto input = "POST / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::post);
}

TEST(RequestParser, Method_Put)
{
	auto input = "PUT / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::put);
}

TEST(RequestParser, Method_Delete)
{
	auto input = "DELETE / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::delete_);
}

TEST(RequestParser, Method_Trace)
{
	auto input = "TRACE / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::trace);
}

TEST(RequestParser, Method_Connect)
{
	auto input = "CONNECT / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.method, zia::api::http::Method::connect);
}

TEST(RequestParser, Version_Unknown)
{
	auto input = "GET / YOLO_VERSION\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::unknown);
}

TEST(RequestParser, Version_Http_0_9)
{
	auto input = "GET / HTTP/0.9\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_0_9);
}

TEST(RequestParser, Version_Http_1_0)
{
	auto input = "GET / HTTP/1.0\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_0);
}

TEST(RequestParser, Version_Http_1_1)
{
	auto input = "GET / HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_1);
}

TEST(RequestParser, Version_Http_2_0)
{
	auto input = "GET / HTTP/2.0\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_2_0);
}