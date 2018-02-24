#include <benchmark/benchmark.h>
#include <cstring>
#include "HttpRequestParser.hpp"

static void BM_HttpRequestParser_VeryShortRequest(benchmark::State& state) {
	auto input = "GET / HTTP/1.1\r\n\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_VeryShortRequest);

static void BM_HttpRequestParser_VeryShortRequest_LongestMethod(benchmark::State& state) {
	auto input = "CONNECT / HTTP/1.1\r\n\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_VeryShortRequest_LongestMethod);

static void BM_HttpRequestParser_VeryShortRequest_MediumUri(benchmark::State& state) {
	auto input = "GET /hello_world.php?s=searching HTTP/1.1\r\n\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_VeryShortRequest_MediumUri);

static void BM_HttpRequestParser_VeryShortRequest_LongUri(benchmark::State& state) {
	auto input = "GET https://ludo:nope@ludonope.com:443/hello_world.php"\
		"?s=searching&q=hello=&f=world#fragment HTTP/1.1\r\n\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_VeryShortRequest_LongUri);

static void BM_HttpRequestParser_ShortRequest1(benchmark::State& state) {
	auto input =
		"GET / HTTP/1.1\r\n" \
		"Content-Length: 40\r\n" \
		"\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_ShortRequest1);

static void BM_HttpRequestParser_ShortRequest2(benchmark::State& state) {
	auto input =
		"GET / HTTP/1.1\r\n" \
		"Date: Sat, 24 Feb 2018 14:52:09 GMT\r\n" \
		"\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_ShortRequest2);

static void BM_HttpRequestParser_NormalRequest(benchmark::State& state) {
	auto input =
		"GET /hello.php?arg=value HTTP/1.1\r\n" \
		"Content-Length: 40\r\n" \
		"Content-Type: text/html\r\n" \
		"Date: Sat, 24 Feb 2018 14:52:09 GMT\r\n" \
		"\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_NormalRequest);

static void BM_HttpRequestParser_LongRequest(benchmark::State& state) {
	auto input =
		"GET https://www.google.fr/search?&q=a+random+google+search HTTP/1.1\r\n" \
		"Cookie: CONSENT=YES+FR.fr+V8; NID=bigHugerandomAndFreakingLongStrangeValueWitchMightBeUsedAsACookieByTheServerFookYeah\r\n" \
		"Dnt: 1\r\n" \
		"Accept-Encoding: gzip, deflate, br\r\n" \
		"Accept-Language: en-US,en;q=0.9,fr;q=0.8\r\n" \
		"Upgrade-Insecure-Requests: 1\r\n" \
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36\r\n" \
		"X-Chrome-Uma-Enabled: 1\r\n" \
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n" \
		"Referer: https://www.google.fr/\r\n" \
		"Authority: www.google.fr\r\n" \
		"X-Client-Data: someEncryptedDataTransfertHere\r\n" \
		"\r\n";

	for (auto _ : state) {
		auto req = zia::http::parseRequest(input);
	}
}
BENCHMARK(BM_HttpRequestParser_LongRequest);

BENCHMARK_MAIN();
