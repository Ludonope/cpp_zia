#if defined __clang__
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif

#include "gtest/gtest.h"
#include "HttpRequestParser.hpp"

TEST(RequestParser, RequestParser_Simple_Valid)
{
	auto input = "GET /path HTTP/1.1\r\n\r\n";
	auto req = zia::http::parseRequest(input);

	ASSERT_EQ(req.version, zia::api::http::Version::http_1_1);
	ASSERT_EQ(req.method, zia::api::http::Method::get);
	ASSERT_EQ(req.uri, "/path");
}
