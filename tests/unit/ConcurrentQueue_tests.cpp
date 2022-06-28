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

#include <chrono>
#include <thread>

TEST(ConcurrentQueue, create)
{
	ConcurrentQueue<int> queue;
	
	ASSERT_EQ(queue.size(), 0);
	ASSERT_EQ(queue.empty(), true);
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

	ASSERT_EQ(*queue.front(), 555);
	ASSERT_EQ(*queue.back(), 555);

	queue.push(928);

	ASSERT_EQ(*queue.front(), 555);
	ASSERT_EQ(*queue.back(), 928);
}

TEST(ConcurrentQueue, pop)
{
	ConcurrentQueue<int> queue;

	queue.push(555);

	ASSERT_EQ(*queue.front(), 555);
	ASSERT_EQ(*queue.back(), 555);

	queue.push(928);

	ASSERT_EQ(*queue.front(), 555);
	ASSERT_EQ(*queue.back(), 928);

	queue.pop();

	ASSERT_EQ(*queue.front(), 928);
	ASSERT_EQ(*queue.back(), 928);
}

TEST(ConcurrentQueue, sequential_usage)
{
	ConcurrentQueue<int> queue;
	const int elems = 5000;
	
	for (int i = 0; i < elems; ++i)
		queue.push(i);

	ASSERT_EQ(queue.size(), elems);
	ASSERT_EQ(queue.empty(), false);
	
	for (int i = 0; i < elems; ++i) {
		ASSERT_EQ(*queue.front(), i);
		queue.pop();
		ASSERT_EQ(queue.size(), elems -i-1);
	}

	ASSERT_EQ(queue.size(), 0);
	ASSERT_EQ(queue.empty(), true);
}


TEST(ConcurrentQueue, concurrent_usage)
{
	ConcurrentQueue<int> queue;
	const int elems = 10000;
	
	std::thread prod([&]() {
		for (int i = 0; i < elems; ++i) {
			queue.push(i);
			std::this_thread::sleep_for(std::chrono::microseconds(2));
		}	
		});

	std::thread cons([&]() {
		int count = 0;
		while (count < elems) {
			if (!queue.empty()) {
				queue.pop();
				count++;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(2));
		}
		});

	prod.join();
	cons.join();

	ASSERT_EQ(queue.empty(), true);
}
