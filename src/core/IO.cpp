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
#include "config/ConfigurationManager.hpp"

#include <thread>
#include <atomic>
#include <functional>

bool IO::configure(ConfigurationManager& config, const std::string& section)
{
	return configuration_.configure(config, section);
}

bool IO::async_read(std::vector<char>& buffer, size_t readMax, rxCallback_t rxCallback)
{
	if (rxCallback == nullptr || rxCallback_ == nullptr)
		return false;

	std::thread read_worker(&IO::async_read_worker, this, std::ref(buffer), readMax, rxCallback == nullptr ? rxCallback_ : rxCallback);
	read_worker.detach();

	return true;
}

bool IO::async_write(const std::vector<char>& buffer, size_t writeMax, txCallback_t txCallback)
{
	if (txCallback == nullptr || txCallback_ == nullptr)
		return false;

	std::thread write_worker(&IO::async_write_worker, this, std::ref(buffer), writeMax, txCallback == nullptr ? txCallback_ : txCallback);
	write_worker.detach();

	return true;
}


void IO::async_read_worker(std::vector<char>& buffer, size_t readMax, rxCallback_t rxCallback)
{
	auto ret = read(buffer, readMax);
	
	rxCallback(buffer, ret);
}

void IO::async_write_worker(const std::vector<char>& buffer, size_t writeMax, txCallback_t txCallback)
{
	auto ret = write(buffer, writeMax);

	txCallback(buffer, ret);
}