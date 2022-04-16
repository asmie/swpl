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
#include <functional>
#include <array>

namespace swpl {

/**
* Stream direction - defines whether stream is only input stream (read), output (write) or 
* bi-directional (read/write).
*/
enum class StreamDirection {
	INPUT = 0,
	OUTPUT,
	BIDIRECTIONAL
};

/**
* Abstrac class with IO interface. Every input/output connector of the application must derive 
* from this interface.
*/
class IO
{
public:
	typedef std::function<void(std::array<char>&, ssize_t)> Callback_t;

	/**
	* Default constructor of the class objects.
	*/
	IO() = default;

	/**
	* Parametrized constructor that sets the basic data.
	* @param[in] id stream identificator
	* @param[in] name name of the stream
	* @param[in] direction stream direction (in/out/in-out)
	*/
	IO(unsigned int id, std::string name, StreamDirection direction) : id_(id), name_(name), direction_(direction) {}
	
	/**
	* Default destructor.
	*/
	virtual ~IO() = default;

	/**
	* Opens stream. Each stream needs to be opened before usage but after configuration.
	* @return 0 if successful, otherwise negative info with error code.
	*/
	virtual int open() = 0;

	/**
	* Closes stream. Any operation after close is not considered valid.
	* @return 0 if successful, otherwise negative info with error code.
	*/
	virtual int close() = 0;

	/**
	* Read at most readMax bytes from the stream and store it in buffer.
	* @param[out] buffer array to store read values
	* @param[in] readMax max bytes to read (must be less than buffer.max_size()).
	* @return Bytes read and stored in the buffer or negative if error occured.
	*/
	virtual ssize_t read(std::array<char>& buffer, size_t readMax = 0) = 0;

	/**
	* Write at most writeMax bytes into the stream from the buffer.
	* @param[in] buffer array to be written into the stream
	* @param[in] writeMax max bytes to be written into the stream
	* @return Bytes written to the stream or negative if error occured.
	*/
	virtual ssize_t write(const std::array<char>& buffer, size_t writeMax = 0) = 0;

	virtual ssize_t async_read(std::array<char>& buffer, size_t readMax = 0) = 0;
	virtual ssize_t async_write(const std::array<char>& buffer, size_t writeMax = 0) = 0;

	virtual bool register_callbacks(Callback_t rxCallback, Callback_t txCallback) = 0;

	virtual bool operator==(const IO &rhs)
	{
		return (rhs.id_ == id_);
	}

	unsigned int getID() const
	{
		return id_;
	}

	std::string getName() const
	{
		return name_;
	}

private:
	unsigned int id_{0};		/*!< IO identification number */
	std::string name_{""};		/*!< Name of the IO object - used for user friendly printing */
	StreamDirection direction_{StreamDirection::BIDIRECTIONAL}; /*!< Direction of the stream */

	Callback_t rxCallback_;
	Callback_t txCallback_;
};

} /* namespace swpl */

#endif /* SRC_IO_HPP_ */
