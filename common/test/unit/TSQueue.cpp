#include <gtest/gtest.h>
#include "TSQueue.hpp"

TEST(TSQueue_suiteTest, simpleAddition)
{
	zia::TSQueue<std::int32_t> queue;

	ASSERT_EQ(queue.size(), 0);
	queue.push(42);
	ASSERT_EQ(queue.size(), 1);
}

TEST(TSQueue_suiteTest, simpleAdditionDeletion)
{
	zia::TSQueue<std::int32_t> queue;

	ASSERT_EQ(queue.size(), 0);
	queue.push(42);
	ASSERT_EQ(queue.size(), 1);
	queue.pop();
	ASSERT_EQ(queue.size(), 0);
	ASSERT_EQ(queue.empty(), true);
}

TEST(TSQueue_suiteTest, multipleAddition)
{
	zia::TSQueue<std::int32_t> queue;

	ASSERT_EQ(queue.empty(), true);
	for (std::int32_t i = 1; i < 10; ++i)
	{
		queue.push(i);
		ASSERT_EQ(queue.size(), i);
	}
	ASSERT_EQ(queue.empty(), false);
}

TEST(TSQueue_suiteTest, BackAndFront)
{
	zia::TSQueue<std::int32_t> queue;

	ASSERT_EQ(queue.empty(), true);
	queue.push(42);
	queue.push(43);
	queue.push(44);
	ASSERT_EQ(queue.size(), 3);
	ASSERT_EQ(queue.front(), 42);
	ASSERT_EQ(queue.back(), 44);
}
