#include <gtest/gtest.h>
#include <cstdint>

#define protected public
#define private public // Evil hack for unit tests
#include "RingBuffer.hpp"

TEST(RingBuffer, WriteSmall)
{
	std::uint8_t	payload[] = {0x00, 0x01, 0x02, 0x03};
	zia::RingBuffer<sizeof(payload)>	smallBuffer = {};
	std::byte *payloadPtr = reinterpret_cast<decltype(payloadPtr)>(payload);

	smallBuffer.write(payloadPtr, sizeof(payload));
	ASSERT_EQ(smallBuffer.m_data[0], payloadPtr[0]);
	ASSERT_EQ(smallBuffer.m_data[1], payloadPtr[1]);
	ASSERT_EQ(smallBuffer.m_data[2], payloadPtr[2]);
	ASSERT_EQ(smallBuffer.m_data[3], payloadPtr[3]);
}

TEST(RingBuffer, WriteBigger)
{
	std::uint8_t	payload[] =
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
	zia::RingBuffer<sizeof(payload) - 3>	smallBuffer = {};
	std::byte *payloadPtr = reinterpret_cast<decltype(payloadPtr)>(payload);

	smallBuffer.write(payloadPtr, sizeof(payload));
	ASSERT_EQ(smallBuffer.m_data[0], payloadPtr[4]);
	ASSERT_EQ(smallBuffer.m_data[1], payloadPtr[5]);
	ASSERT_EQ(smallBuffer.m_data[2], payloadPtr[6]);
	ASSERT_EQ(smallBuffer.m_data[3], payloadPtr[3]);
}

TEST(RingBuffer, WriteAndRead)
{
	std::uint8_t	payload[] = {0x00, 0x01, 0x02, 0x03};
	zia::RingBuffer<sizeof(payload)>	smallBuffer = {};
	std::byte *payloadPtr = reinterpret_cast<decltype(payloadPtr)>(payload);

	smallBuffer.write(payloadPtr, sizeof(payload));
	std::byte	data[4] = {};
	smallBuffer.read(data, 2);
	ASSERT_EQ(data[0], payloadPtr[0]);
	ASSERT_EQ(data[1], payloadPtr[1]);
	smallBuffer.read(data, 2);
	ASSERT_EQ(data[0], payloadPtr[2]);
	ASSERT_EQ(data[1], payloadPtr[3]);
}

TEST(RingBuffer, GetAvailableData_WriteOnly)
{
	std::uint8_t	payload[] = {0x00, 0x01, 0x02, 0x03};
	zia::RingBuffer<5>	smallBuffer = {};
	std::byte *payloadPtr = reinterpret_cast<decltype(payloadPtr)>(payload);

	// x|x|x|x|x
	ASSERT_EQ(smallBuffer.m_ndxWrite, 0);
	ASSERT_EQ(smallBuffer.getAvailableData(), 0);

	// o|x|x|x|x
	smallBuffer.write(payloadPtr, 1);
	ASSERT_EQ(smallBuffer.m_ndxWrite, 1);
	ASSERT_EQ(smallBuffer.getAvailableData(), 1);

	// o|o|x|x|x
	smallBuffer.write(payloadPtr, 1);
	ASSERT_EQ(smallBuffer.m_ndxWrite, 2);
	ASSERT_EQ(smallBuffer.getAvailableData(), 2);

	// o|o|o|o|o
	smallBuffer.write(payloadPtr, 3);
	// m_ndxWrite will be reseted to 0 during first loop
	ASSERT_EQ(smallBuffer.m_ndxWrite, 5);
	ASSERT_EQ(smallBuffer.getAvailableData(), 5);
}

TEST(RingBuffer, GetAvailableData_RW)
{
	std::uint8_t	payload[] = {0x00, 0x01, 0x02, 0x03};
	zia::RingBuffer<5>	smallBuffer = {};
	std::byte *payloadPtr = reinterpret_cast<decltype(payloadPtr)>(payload);
	std::byte tmp[5] = {};

	// x|x|x|x|u
	smallBuffer.write(payloadPtr, 4);
	ASSERT_EQ(smallBuffer.getAvailableData(), 4);

	// o|o|x|x|u
	smallBuffer.read(tmp, 2);
	ASSERT_EQ(smallBuffer.getAvailableData(), 2);

	// o|o|o|x|u
	smallBuffer.read(tmp, 1);
	ASSERT_EQ(smallBuffer.getAvailableData(), 1);

	// o|o|o|x|x
	smallBuffer.write(payloadPtr, 1);
	ASSERT_EQ(smallBuffer.getAvailableData(), 2);
}

TEST(RingBuffer, Peek)
{
	std::uint8_t	payload[] = {0x00, 0x01, 0x02, 0x03};
	zia::RingBuffer<5>	smallBuffer = {};
	std::byte *payloadPtr = reinterpret_cast<decltype(payloadPtr)>(payload);
	std::vector<std::byte> cmp = {};

	ASSERT_EQ(smallBuffer.peek(), cmp);
	smallBuffer.write(payloadPtr, 1);
	cmp.push_back(payloadPtr[0]);
	ASSERT_EQ(smallBuffer.peek(), cmp);
	smallBuffer.write(payloadPtr + 1, 2);
	cmp.push_back(payloadPtr[1]);
	cmp.push_back(payloadPtr[2]);
	ASSERT_EQ(smallBuffer.peek(), cmp);
}
