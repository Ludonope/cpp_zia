#if defined __clang__
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif

#include <string>
#include "gtest/gtest.h"
#include "ResponseSerializer.hpp"
#include "api/http.h"

using std::operator""s;
using zia::api::HttpResponse;

TEST(ResponseSerializer, NormalResponse)
{
	auto res = HttpResponse{
		zia::api::http::Version::http_1_1,
		{
			{ "Content-Length", "2" },
			{ "Content-Type", "text/html" }
		},
		{ std::byte{'4'}, std::byte{'2'} },
		zia::api::http::common_status::ok,
		"OK"
	};
	auto str = zia::http::responseToString(res);
	auto expected = 
		"HTTP/1.1 200 OK\r\n" \
		"Content-Length: 2\r\n" \
		"Content-Type: text/html\r\n" \
		"\r\n" \
		"42";

	ASSERT_EQ(str, expected);
}

TEST(ResponseSerializer, NoHeader)
{
	auto res = HttpResponse{
		zia::api::http::Version::http_1_1,
	{ },
	{ },
		zia::api::http::common_status::ok,
		"OK"
	};
	auto str = zia::http::responseToString(res);
	auto expected =
		"HTTP/1.1 200 OK\r\n" \
		"\r\n";

	ASSERT_EQ(str, expected);
}