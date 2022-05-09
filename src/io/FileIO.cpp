/**
 *  @file   FileIO.cpp
 *  @brief  File input/output .
 *
 *  @author Piotr Olszewski
 *
 *  @date   2022.04.22
 *
 *  This file contains the class for manipulating files.
 */

#include "FileIO.hpp"
#include "config/ConfigurationManager.hpp"

#include <fstream>
#include <cerrno>
#include <mutex>
#include <string_view>

constexpr std::string_view SETT_TYPE = "type";
constexpr std::string_view SETT_FILE = "file";


FileIO::~FileIO() 
{

}

bool FileIO::configure(ConfigurationManager& config, const std::string& section)
{
	bool configurationCorrect = false;
	std::string val;

	bool res = config.get(section, "type", val);

	if (val != "file")
	{
		configurationCorrect = false;
	}

	return configurationCorrect;
}

int FileIO::open() 
{
	int result = 0;

	if (!fileStream_.is_open())
	{
		std::fstream::openmode mode = static_cast<std::fstream::openmode>(0);
		switch (getConfiguration().getDirection())
		{
			case StreamDirection::BIDIRECTIONAL:
				mode = std::fstream::in | std::fstream::out; break;
			case StreamDirection::INPUT:
				mode = std::fstream::in; break;
			case StreamDirection::OUTPUT:
				mode = std::fstream::out; break;
		}

		try 
		{
			fileStream_.open(fileName_, mode);
			if (!fileStream_.good())
				result = -1;
		}
		catch (std::fstream::failure& e)
		{
			result = -ENFILE;
		}
	}

	return result;
}

int FileIO::close() 
{
	fileStream_.close();
	return 0;
}

ssize_t FileIO::read(std::vector<char>& buffer, size_t readMax) 
{
	ssize_t retVal = 0;

	std::lock_guard<std::mutex> guard(readLock_);

	if (getConfiguration().getDirection() == StreamDirection::OUTPUT)
		return -EINVAL;

	if (!fileStream_.is_open() || !fileStream_.good())
		return -ENFILE;
	
	size_t toRead = buffer.capacity();

	if (readMax != 0 && readMax < toRead)
		toRead = readMax;

	try
	{
		fileStream_.read(buffer.data(), toRead);
		retVal = fileStream_.gcount();
	}
	catch (std::fstream::failure& e)
	{
		retVal = -EBADF;
	}

	return retVal;
}

ssize_t FileIO::write(const std::vector<char>& buffer, size_t writeMax) 
{
	ssize_t retVal = 0;

	std::lock_guard<std::mutex> guard(writeLock_);

	if (getConfiguration().getDirection() == StreamDirection::INPUT)
		return -EINVAL;

	if (!fileStream_.is_open() || !fileStream_.good())
		return -ENFILE;

	size_t toWrite = buffer.capacity();

	if (writeMax != 0 && writeMax < toWrite)
		toWrite = writeMax;

	try
	{
		fileStream_.write(buffer.data(), toWrite);
		retVal = fileStream_.gcount();
	}
	catch (std::fstream::failure& e)
	{
		retVal = -EBADF;
	}

	return retVal;
}