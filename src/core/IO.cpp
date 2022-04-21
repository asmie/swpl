/**
 *  @file   IO.cpp
 *  @brief  Input/output basic data implementation .
 *
 *  @author Piotr Olszewski
 *
 *  @date   2022.04.21
 *
 *  This file contains the base class for any I/O that can be connected to the swpl application.
 */

#include "IO.hpp"
using namespace swpl;

#include <thread>
#include <atomic>

bool IO::async_read(std::array<char>& buffer, size_t readMax = 0, Callback_t rxCallback = nullptr)
{
	if (rxCallback == nullptr || rxCallback_ == nullptr)
		return false;

	std::thread read_worker(&IO::async_read_worker, this, buffer, readMax, rxCallback == nullptr ? rxCallback_ : rxCallback);
	read_worker.detach();

	return true;
}

bool IO::async_write(const std::array<char>& buffer, size_t writeMax = 0, Callback_t txCallback = nullptr)
{
	if (txCallback == nullptr || txCallback_ == nullptr)
		return false;

	std::thread write_worker(&IO::async_write_worker, this, buffer, writeMax, txCallback == nullptr ? txCallback_ : txCallback);
	read_worker.detach();

	return true;
}


void IO::async_read_worker(std::array<char>& buffer, size_t readMax = 0, Callback_t rxCallback = nullptr)
{
	auto ret = read(buffer, readMax);
	
	rxCallback(buffer, ret);
}

void IO::async_write_worker(const std::array<char>& buffer, size_t writeMax = 0, Callback_t txCallback = nullptr)
{
	auto ret = write(buffer, readMax);

	txCallback(buffer, ret);
}