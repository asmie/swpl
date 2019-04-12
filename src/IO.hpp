/**
 *  @file    IO.hpp
 *  @brief  Input/output data model .
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 *
 *  This file contains the base class for any I/O that can be connected to the swpl application.
 */

#ifndef SRC_IO_HPP_
#define SRC_IO_HPP_

#include <cstdlib>
#include <string>

namespace swpl {

enum class StreamDirection {
	INPUT = 0,
	OUTPUT,
	BIDIRECTIONAL
};

class IO
{
public:
	IO() {}
	IO(unsigned int id, std::string name, StreamDirection direction) : _id(id), _name(name), _direction(direction) {}
	virtual ~IO() {}

	virtual int open() = 0;
	virtual int close() = 0;

	virtual ssize_t read(char *buffer, size_t bufferSize, size_t readMax = 0) = 0;

	//template <typename T>
	//virtual ssize_t read(T& buffer, size_t bufferSize, size_t readMax = 0) = 0;

	virtual ssize_t write(const char *buffer, size_t bufferSize) = 0;

	virtual ssize_t async_read(char *buffer, size_t bufferSize, size_t readMax = 0) = 0;
	virtual ssize_t async_write(const char *buffer, size_t bufferSize) = 0;

	virtual bool operator==(const IO &rhs)
	{
		return (rhs._id == _id);
	}

	unsigned int getID() const
	{
		return _id;
	}

	std::string getName() const
	{
		return _name;
	}

private:
	unsigned int _id{0}; /*!< IO identification number */
	std::string _name{""}; /*!< Name of the IO object - used for user friendly printing */
	StreamDirection _direction{StreamDirection::BIDIRECTIONAL}; /*!< Direction of the stream */

};

} /* namespace swpl */

#endif /* SRC_IO_HPP_ */
