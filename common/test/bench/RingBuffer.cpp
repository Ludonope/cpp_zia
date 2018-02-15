#include <benchmark/benchmark.h>
#include <array>
#include "RingBuffer.hpp"

// Create buffer
template <std::size_t BufferSize> 
static void BM_RingBuffer_CreateBuffer(benchmark::State& state) {
	for (auto _ : state) {
		zia::RingBuffer<BufferSize>	buffer;
		benchmark::DoNotOptimize(buffer);
		benchmark::ClobberMemory();
	}
}
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 0);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 1);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 2);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 4);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 8);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 16);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 32);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 64);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 128);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 256);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 512);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 1024);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 2048);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 4096);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 8192);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 16384);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 32768);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 65536);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 131072);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 262144);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 524288);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 1048576);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 2097152);
BENCHMARK_TEMPLATE(BM_RingBuffer_CreateBuffer, 4194304);

// Write to buffer
template<std::size_t BufferSize, std::size_t WriteSize>
static void BM_RingBuffer_Write(benchmark::State &state) {
	zia::RingBuffer<BufferSize>	buffer;
	std::array<std::byte, WriteSize> const data = {};

	for (auto _ : state) {
		benchmark::DoNotOptimize(buffer);
		benchmark::DoNotOptimize(data);
		buffer.write(data.data(), sizeof(data));
		benchmark::ClobberMemory();
	}
}
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 0);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 1);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 2);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 4);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 8);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 16);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 32);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 64);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 128);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 256);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 512);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 1024);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 2048);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 4096);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 16384);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 32768);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 65536);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 131072);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 262144);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 524288);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 1048576);
BENCHMARK_TEMPLATE(BM_RingBuffer_Write, 4096, 2097152);

// Write to buffer and read from it
template<std::size_t BufferSize, std::size_t WriteSize, std::size_t ReadSize>
static void BM_RingBuffer_WriteAndRead(benchmark::State &state) {
	zia::RingBuffer<BufferSize>	buffer;
	std::array<std::byte, WriteSize> const data = {};
	std::array<std::byte, ReadSize> dataToRead = {};

	for (auto _ : state) {
		benchmark::DoNotOptimize(buffer);
		benchmark::DoNotOptimize(data);
		buffer.write(data.data(), sizeof(data));
		benchmark::ClobberMemory();
		buffer.read(dataToRead.data(), sizeof(dataToRead));
		benchmark::ClobberMemory();
	}
}
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 0);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 1);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 2);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 4);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 8);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 16);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 32);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 64);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 128);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 256);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 512);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 1024);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 2048);
BENCHMARK_TEMPLATE(BM_RingBuffer_WriteAndRead, 4096, 4096, 4096);

// Get available data
template<std::size_t BufferSize, std::size_t WriteSize>
static void BM_RingBuffer_GetAvailableData(benchmark::State &state) {
	zia::RingBuffer<BufferSize>	buffer;
	std::array<std::byte, WriteSize> const data = {};

	buffer.write(data.data(), sizeof(data));
	benchmark::ClobberMemory();
	for (auto _ : state) {
		benchmark::DoNotOptimize(buffer);
		benchmark::DoNotOptimize(data);
		auto const available = buffer.getAvailableData();
		benchmark::DoNotOptimize(available);
	}
}
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 0);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 1);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 2);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 4);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 8);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 16);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 32);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 64);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 128);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 256);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 512);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 1024);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 2048);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 4096);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 8192);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 16384);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 32768);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 65536);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 131072);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 262144);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 524288);
BENCHMARK_TEMPLATE(BM_RingBuffer_GetAvailableData, 1048576, 1048576);

// Peek data from the buffer
template<std::size_t BufferSize, std::size_t WriteSize>
static void BM_RingBuffer_PeekData(benchmark::State &state) {
	zia::RingBuffer<BufferSize>	buffer;
	std::array<std::byte, WriteSize> const data = {};

	buffer.write(data.data(), sizeof(data));
	benchmark::ClobberMemory();
	for (auto _ : state) {
		benchmark::DoNotOptimize(buffer);
		benchmark::DoNotOptimize(data);
		auto const available = buffer.peek();
		benchmark::DoNotOptimize(available);
	}
}
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 0);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 1);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 2);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 4);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 8);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 16);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 32);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 64);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 128);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 256);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 512);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 1024);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 2048);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 4096);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 8192);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 16384);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 32768);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 65536);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 131072);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 262144);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 524288);
BENCHMARK_TEMPLATE(BM_RingBuffer_PeekData, 1048576, 1048576);

BENCHMARK_MAIN();
