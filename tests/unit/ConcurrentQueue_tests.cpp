/**
 *  @file   ConcurrentQueue_tests.cpp
 *  @brief  Unit tests for ConcurrentQueue component.
 *
 *  @author Piotr Olszewski     asmie@asmie.pl
 *
 *  @date   2022.06.28
 *
 * Unit tests for ConcurrentQueue.
 *
 */

#include "gtest/gtest.h"
#include "core/ConcurrentQueue.hpp"

TEST(ConcurrentQueue, create)
{
	ConcurrentQueue<int> queue;
	
}

TEST(ConcurrentQueue, size)
{
	ConcurrentQueue<int> queue;
	
	ASSERT_EQ(queue.size(), 0);
	
	queue.push(555);
	
	ASSERT_EQ(queue.size(), 1);
}

TEST(ConcurrentQueue, push)
{
	ConcurrentQueue<int> queue;

	queue.push(555);

	ASSERT_EQ(queue.front(), 555);
	ASSERT_EQ(queue.back(), 555);

	queue.push(928);

	ASSERT_EQ(queue.front(), 555);
	ASSERT_EQ(queue.back(), 928);
}

TEST(ConcurrentQueue, pop)
{
	ConcurrentQueue<int> queue;

	queue.push(555);

	ASSERT_EQ(queue.front(), 555);
	ASSERT_EQ(queue.back(), 555);

	queue.push(928);

	ASSERT_EQ(queue.front(), 555);
	ASSERT_EQ(queue.back(), 928);

	queue.pop();

	ASSERT_EQ(queue.front(), 928);
	ASSERT_EQ(queue.back(), 928);
}

TEST(ConcurrentQueue, sequential_usage)
{

}


TEST(ConcurrentQueue, concurrent_usage)
{

}
