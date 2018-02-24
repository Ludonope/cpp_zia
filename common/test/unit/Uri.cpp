#if defined __clang__
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif

#include "gtest/gtest.h"
#include "Uri.hpp"

TEST(Uri, BasicUriParsing)
{
	auto str = "http://ludonope.com/search.php?s=hello";
	auto uri = zia::http::Uri(str);

	ASSERT_EQ(uri.scheme(), "http");
	ASSERT_EQ(uri.host(), "ludonope.com");
	ASSERT_EQ(uri.path(), "search.php");
	ASSERT_EQ(uri.query(), "s=hello");
}

TEST(Uri, PathParsing)
{
	auto str = std::string_view("/path/to/custom_user-file.txt.cvs");
	auto uri = zia::http::Uri(str);

	ASSERT_EQ(uri.path(), str.substr(1));
}

TEST(Uri, CompleteUriParsing)
{
	auto str = "http://ludo:nope@ludonope.com:4242/search.php?s=hello#result";
	auto uri = zia::http::Uri(str);

	ASSERT_EQ(uri.scheme(), "http");
	ASSERT_EQ(uri.user(), "ludo");
	ASSERT_EQ(uri.password(), "nope");
	ASSERT_EQ(uri.host(), "ludonope.com");
	ASSERT_EQ(uri.port(), 4242);
	ASSERT_EQ(uri.path(), "search.php");
	ASSERT_EQ(uri.query(), "s=hello");
	ASSERT_EQ(uri.fragment(), "result");
}

TEST(Uri, ToString)
{
	auto str = "http://ludo:nope@ludonope.com:4242/search.php?s=hello#result";
	auto uri = zia::http::Uri(str);
	auto res = uri.toString();

	ASSERT_EQ(str, res);
}

TEST(Uri, UriDecoding)
{
	auto str = "http://lu%20do:nop%2ce@lu%2Bdo%2Fnop%3Fe.com:4242/s%40earch.php?s=hello%20world#re%23sult";
	auto uri = zia::http::Uri(str);

	ASSERT_EQ(uri.scheme(), "http");
	ASSERT_EQ(uri.user(), "lu do");
	ASSERT_EQ(uri.password(), "nop,e");
	ASSERT_EQ(uri.host(), "lu+do/nop?e.com");
	ASSERT_EQ(uri.port(), 4242);
	ASSERT_EQ(uri.path(), "s@earch.php");
	ASSERT_EQ(uri.query(), "s=hello%20world");
	ASSERT_EQ(zia::http::Uri::decode(uri.query()), "s=hello world");
	ASSERT_EQ(uri.fragment(), "re#sult");
}

TEST(Uri, Normalization)
{
	auto str = "HTtp://ludoNOPE.cOm/sEArch.pHp";
	auto uri = zia::http::Uri(str);

	ASSERT_EQ(uri.scheme(), "http");
	ASSERT_EQ(uri.host(), "ludonope.com");
	ASSERT_EQ(uri.path(), "sEArch.pHp");
}