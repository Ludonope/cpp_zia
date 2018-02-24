#include <benchmark/benchmark.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "ResponseSerializer.hpp"

static void BM_ResponseSerializer_VeryShortResponse(benchmark::State& state) {
	auto res = zia::api::HttpResponse{
		zia::api::http::Version::http_1_1,
		{ },
		{ },
		zia::api::http::common_status::ok,
		"OK"
	};

	for (auto _ : state) {
		auto req = zia::http::responseToString(res);
	}
}
BENCHMARK(BM_ResponseSerializer_VeryShortResponse);

template <std::size_t ContentSize>
static void BM_ResponseSerializer_ContentResponse(benchmark::State& state) {
	auto body = std::vector<std::byte>{};
	
	std::srand(std::time(nullptr));
	body.resize(ContentSize);

	for (auto &val : body)
	{
		val = static_cast<std::byte>(std::rand() % 256);
	}

	auto res = zia::api::HttpResponse{
		zia::api::http::Version::http_1_1,
		{
			{ "Content-Length", std::to_string(ContentSize) },
			{ "Content-Type", "text/html" }
		},
		body,
		zia::api::http::common_status::ok,
		"OK"
	};

	for (auto _ : state) {
		auto req = zia::http::responseToString(res);
	}
}
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 0);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 1);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 2);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 4);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 8);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 16);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 32);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 64);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 128);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 256);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 512);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 1024);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 2048);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 4096);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 8192);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 16384);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 32768);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 65536);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 131072);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 262144);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 524288);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 1048576);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 2097152);
BENCHMARK_TEMPLATE(BM_ResponseSerializer_ContentResponse, 4194304);

BENCHMARK_MAIN();
