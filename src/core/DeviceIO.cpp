/**
 *  @file   DeviceIO.cpp
 *  @brief  Device input/output .
 *
 *  @author Piotr Olszewski
 *
 *  @date   2022.04.22
 *
 *  This file contains the class for manipulating devices.
 * 
 *  This implementation contains different code for different OS. It may seem 
 *  to be not consistent with the fact that there is osdep directory within
 *  the project and therefore, OS-dependent implementation should be put there but
 *  the differences in open/close/read/write code was so small that it could be 
 *  really over-sized to implement wrappers under osdep files. Moreover, choosing 
 *  function during compile time here is for sure more optimized approach than adding
 *  extra call to OS abstraction layer just to simply call other function with slightly 
 *  different name.
 */

#include "DeviceIO.hpp"
#include "Global.h"

#include <cerrno>
#include <mutex>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef SWPL_SYSTEM_HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef SWPL_SYSTEM_HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef SWPL_SYSTEM_HAVE_IO_H
#include <io.h>
#endif


DeviceIO::~DeviceIO()
{

}

bool DeviceIO::configure(ConfigurationManager& config, const std::string section)
{
	bool configurationCorrect = false;

	return configurationCorrect;
}

int DeviceIO::open()
{
	int result = 0;

	if (devFd_ < 0)
	{
		int flags = 0;
		switch (getDirection())
		{
		case StreamDirection::BIDIRECTIONAL:
			flags = O_RDWR; break;
		case StreamDirection::INPUT:
			flags = O_RDONLY; break;
		case StreamDirection::OUTPUT:
			flags = O_WRONLY; break;
		}

		errno = 0;

#if defined(SWPL_SYSTEM_HAVE_IO_H)
		if (_sopen_s(&devFd_, devicePath_.c_str(), flags, 0, 0) != 0)
			result = -errno;
#elif defined(SWPL_SYSTEM_HAVE_UNISTD_H)
		devFd_ = ::open(devicePath_.c_str(), flags);
		if (devFd_ < 0)
			result = -errno;
#else
#error "Can't use DeviceIO module because no open call is available"
#endif
	}

	return result;
}

int DeviceIO::close()
{
#if defined(SWPL_SYSTEM_HAVE_IO_H)
	if (_close(devFd_) == -1)
		return -errno;
#elif defined(SWPL_SYSTEM_HAVE_UNISTD_H)
	if (::close(devFd_) == -1)
		return -errno;
#else
#error "Can't use DeviceIO module because no close call is available"
#endif
	return 0;
}

ssize_t DeviceIO::read(std::vector<char>& buffer, size_t readMax)
{
	ssize_t retVal = 0;

	std::lock_guard<std::mutex> guard(readLock_);

	if (getDirection() == StreamDirection::OUTPUT)
		return -EINVAL;

	if (devFd_ < 0)
		return -ENFILE;

	size_t toRead = buffer.capacity();

	if (readMax != 0 && readMax < toRead)
		toRead = readMax;

#if defined(SWPL_SYSTEM_HAVE_IO_H)
	retVal = _read(devFd_, buffer.data(), toRead);
#elif defined(SWPL_SYSTEM_HAVE_UNISTD_H)
	retVal = ::read(devFd_, buffer.data(), toRead);
#else
#error "Can't use DeviceIO module because no read call is available"
#endif

	if (retVal < 0)
		retVal = -errno;

	return retVal;
}

ssize_t DeviceIO::write(const std::vector<char>& buffer, size_t writeMax)
{
	ssize_t retVal = 0;

	std::lock_guard<std::mutex> guard(writeLock_);

	if (getDirection() == StreamDirection::INPUT)
		return -EINVAL;

	if (devFd_ < 0)
		return -ENFILE;

	size_t toWrite = buffer.capacity();

	if (writeMax != 0 && writeMax < toWrite)
		toWrite = writeMax;

#if defined(SWPL_SYSTEM_HAVE_IO_H)
	retVal = _write(devFd_, buffer.data(), toWrite);
#elif defined(SWPL_SYSTEM_HAVE_UNISTD_H)
	retVal = ::write(devFd_, buffer.data(), toWrite);
#else
#error "Can't use DeviceIO module because no write call is available"
#endif
	
	if (retVal < 0)
		retVal = -errno;

	return retVal;
}