#include <benchmark/benchmark.h>
#include "Uri.hpp"

static void BM_Uri_UltraShort(benchmark::State& state) {
	auto input = "/";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_UltraShort);

static void BM_Uri_FilePath(benchmark::State& state) {
	auto input = "/path/to/a/file";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_FilePath);

static void BM_Uri_CommonUri(benchmark::State& state) {
	auto input = "http://ludonope.com/";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_CommonUri);

static void BM_Uri_CommonUri_WithQuery(benchmark::State& state) {
	auto input = "http://ludonope.com/search.php?s=hello%20world";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_CommonUri_WithQuery);

static void BM_Uri_CommonUri_WithFragment(benchmark::State& state) {
	auto input = "http://ludonope.com/search.php#fragment";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_CommonUri_WithFragment);

static void BM_Uri_CommonUri_WithQueryAndFragment(benchmark::State& state) {
	auto input = "http://ludonope.com/search.php?s=hello%20world#fragment";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_CommonUri_WithQueryAndFragment);

static void BM_Uri_CommonUri_WithHostGroup(benchmark::State& state) {
	auto input = "https://ludo:nope@ludonope.com:443/";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_CommonUri_WithHostGroup);

static void BM_Uri_CompleteUri(benchmark::State& state) {
	auto input = "https://ludo:nope@ludonope.com:443/search.php?s=hello%20world#fragment";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_CompleteUri);

static void BM_Uri_LargeUri(benchmark::State& state) {
	auto input = "https://ludoooooooooooo:nopeeeeeeeeeeee@ludonopeeeeeeeeeeeeeeeeeeeee.com" \
		":443/searchhhhhhhhhh.php?ssssssssssssssss=hello%20world%20hello%20world%20" \
		"hello%20world%20hello%20world%20hello%20world%20hello%20world%20hello%20world%20" \
		"#fragment_fragment_fragment_fragment_fragment_fragment_fragment_fragment_fragment";

	for (auto _ : state) {
		auto uri = zia::http::Uri(input);
	}
}
BENCHMARK(BM_Uri_LargeUri);

BENCHMARK_MAIN();
