#include <benchmark/benchmark.h>
#include <cstring>
#include "HttpRingBuffer.hpp"

static void BM_HttpRingBuffer_CreateBuffer(benchmark::State& state) {
	for (auto _ : state) {
		zia::network::HttpRingBuffer	buffer;
		benchmark::DoNotOptimize(buffer);
		benchmark::ClobberMemory();
	}
}
BENCHMARK(BM_HttpRingBuffer_CreateBuffer);

static void BM_HttpRingBuffer_EmptyHeader(benchmark::State& state) {
	zia::network::HttpRingBuffer	buffer;

	for (auto _ : state) {
		buffer.hasHeader();
	}
}
BENCHMARK(BM_HttpRingBuffer_EmptyHeader);

static void BM_HttpRingBuffer_HeaderTrash(benchmark::State& state) {
	char const *payload = "Hello World !";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));

	for (auto _ : state) {
		buffer.hasHeader();
	}
}
BENCHMARK(BM_HttpRingBuffer_HeaderTrash);

static void BM_HttpRingBuffer_HeaderValid(benchmark::State& state) {
	char const *payload =
		"GET /hello.htm HTTP/1.1\r\n"
		"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
		"Host: www.tutorialspoint.com\r\n"
		"Accept-Language: en-us\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Connection: Keep-Alive\r\n"
		"\r\n";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	for (auto _ : state) {
		buffer.hasHeader();
	}
}
BENCHMARK(BM_HttpRingBuffer_HeaderValid);

static void BM_HttpRingBuffer_HeaderValidWithContentLength(benchmark::State& state) {
	char const *payload =
		"POST /path/script.cgi HTTP/1.0\r\n"
		"From: frog@jmarshall.com\r\n"
		"User-Agent: HTTPTool/1.0\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"Content-Length: 32\r\n"
		"\r\n"
		"home=Cosby&favorite+flavor=flies";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	for (auto _ : state) {
		buffer.hasHeader();
	}
}
BENCHMARK(BM_HttpRingBuffer_HeaderValidWithContentLength);

static void BM_HttpRingBuffer_WriteRequestValid(benchmark::State& state) {
	for (auto _ : state) {
		char const *payload =
			"GET /hello.htm HTTP/1.1\r\n"
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
			"Host: www.tutorialspoint.com\r\n"
			"Accept-Language: en-us\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Connection: Keep-Alive\r\n"
			"\r\n";
		std::byte const *payloadPtr =
			reinterpret_cast<decltype(payloadPtr)>(payload);
		zia::network::HttpRingBuffer	buffer;

		buffer.write(payloadPtr, std::strlen(payload));
	}	
}
BENCHMARK(BM_HttpRingBuffer_WriteRequestValid);

static void BM_HttpRingBuffer_GetRequestValid(benchmark::State& state) {
	for (auto _ : state) {
		char const *payload =
			"GET /hello.htm HTTP/1.1\r\n"
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
			"Host: www.tutorialspoint.com\r\n"
			"Accept-Language: en-us\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Connection: Keep-Alive\r\n"
			"\r\n";
		std::byte const *payloadPtr =
			reinterpret_cast<decltype(payloadPtr)>(payload);
		zia::network::HttpRingBuffer	buffer;

		buffer.write(payloadPtr, std::strlen(payload));
		auto const req = buffer.getRequest();
	}
}
BENCHMARK(BM_HttpRingBuffer_GetRequestValid);

static void BM_HttpRingBuffer_WriteRequestValidWithContentLength(benchmark::State& state) {
	for (auto _ : state) {
		char const *payload =
			"POST /path/script.cgi HTTP/1.0\r\n"
			"From: frog@jmarshall.com\r\n"
			"User-Agent: HTTPTool/1.0\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Content-Length: 32\r\n"
			"\r\n"
			"home=Cosby&favorite+flavor=flies";
		std::byte const *payloadPtr =
			reinterpret_cast<decltype(payloadPtr)>(payload);
		zia::network::HttpRingBuffer	buffer;

		buffer.write(payloadPtr, std::strlen(payload));
	}	
}
BENCHMARK(BM_HttpRingBuffer_WriteRequestValidWithContentLength);

static void BM_HttpRingBuffer_GetRequestValidWithContentLength(benchmark::State& state) {
	for (auto _ : state) {
		char const *payload =
			"POST /path/script.cgi HTTP/1.0\r\n"
			"From: frog@jmarshall.com\r\n"
			"User-Agent: HTTPTool/1.0\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Content-Length: 32\r\n"
			"\r\n"
			"home=Cosby&favorite+flavor=flies";
		std::byte const *payloadPtr =
			reinterpret_cast<decltype(payloadPtr)>(payload);
		zia::network::HttpRingBuffer	buffer;
		buffer.write(payloadPtr, std::strlen(payload));
		auto const req = buffer.getRequest();
	}
}
BENCHMARK(BM_HttpRingBuffer_GetRequestValidWithContentLength);

BENCHMARK_MAIN();
