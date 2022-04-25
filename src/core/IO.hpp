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

#include "Global.h"

#include <cstdlib>
#include <string>
#include <string_view>
#include <functional>
#include <vector>

class ConfigurationManager;

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
	typedef std::function<void(std::vector<char>&, ssize_t)> rxCallback_t;
	typedef std::function<void(const std::vector<char>&, ssize_t)> txCallback_t;

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
	IO(unsigned int id, const std::string& name, StreamDirection direction) : id_(id), name_(name), direction_(direction), rxCallback_(nullptr), txCallback_(nullptr) {}
	
	/**
	* Default destructor.
	*/
	virtual ~IO() = default;

	/**
	* Method allowing module to configure itself using external configuration source.
	* @param[in] config reference to the configuration manager facility
	* @param[in] section place where module configuration is stored
	* @return True if configuration is valid, otherwise false.
	*/
	virtual bool configure(ConfigurationManager& config, const std::string& section) = 0;

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
	virtual ssize_t read(std::vector<char>& buffer, size_t readMax = 0) = 0;

	/**
	* Write at most writeMax bytes into the stream from the buffer.
	* @param[in] buffer array to be written into the stream
	* @param[in] writeMax max bytes to be written into the stream
	* @return Bytes written to the stream or negative if error occured.
	*/
	virtual ssize_t write(const std::vector<char>& buffer, size_t writeMax = 0) = 0;

	/**
	* Async read method. Takes reference to buffer and two optional parameters - max read chars 
	* (readMax <= buffer.max_size()) and rxCallback. If no callback is given then it is used default one
	* that should be set using register_callbacks() method. If there is still no possible callback then
	* method will fail. This is the default implementation.
	* @param[out] buffer buffer where read bytes will be stored
	* @param[in] readMax read max bytes - 0 means read buffer.max_size() bytes
	* @param[in] rxCallback call rxCallback instead of remembered function
	* @return True if everything is ok, otherwise false.l
	*/
	virtual bool async_read(std::vector<char>& buffer, size_t readMax = 0, rxCallback_t rxCallback = nullptr);

	/**
	* Async write method. Takes reference to buffer and two optional parameters - max write bytes
	* (writedMax <= buffer.max_size()) and txCallback. If no callback is given then it is used default one
	* that should be set using register_callbacks() method. If there is still no possible callback then
	* method will fail. This is the default implementation.
	* @param[in] buffer buffer where read bytes will be stored
	* @param[in] readMax read max bytes - 0 means read buffer.max_size() bytes
	* @param[in] rxCallback call rxCallback instead of remembered function
	* @return True if everything is ok, otherwise false.
	*/
	virtual bool async_write(const std::vector<char>& buffer, size_t writeMax = 0, txCallback_t txCallback = nullptr);

	/**
	* Set RX callback to be executed upon async_read is completed.
	* @param[in] rxCallback callback to call when async_read is completed.
	*/
	virtual void register_rx_callback(rxCallback_t rxCallback)
	{
		rxCallback_ = rxCallback;
	}

	/**
	* Set TX callback to be executed when async_write is completed.
	* @param[in] txCallback callback to be called when async_write is completed.
	*/
	virtual void register_tx_callback(txCallback_t txCallback)
	{
		txCallback_ = txCallback;
	}

	/**
	* Check if two streams are the same.
	* @param[in] rhs IO object to be compared
	* @return True if streams have the same ID, otherwise false.
	*/
	virtual bool operator==(const IO &rhs)
	{
		return (rhs.id_ == id_);
	}

	/**
	* Get IO object ID.
	* @return ID of the object.
	*/
	unsigned int getID() const
	{
		return id_;
	}

	/**
	* Get name of the IO object.
	* @return Name of the IO object.
	*/
	std::string getName() const
	{
		return name_;
	}

	/**
	* Get stream direction.
	* @return Stream direction.
	*/
	StreamDirection getDirection() const
	{
		return direction_;
	}

private:
	/**
	* Helper method to async reading.
	*/
	virtual void async_read_worker(std::vector<char>& buffer, size_t readMax = 0, rxCallback_t rxCallback = nullptr);

	/**
	* Helper method to async writing.
	*/
	virtual void async_write_worker(const std::vector<char>& buffer, size_t writeMax = 0, txCallback_t txCallback = nullptr);


	unsigned int id_{0};		/*!< IO identification number */
	std::string name_{""};		/*!< Name of the IO object - used for user friendly printing */
	StreamDirection direction_{StreamDirection::BIDIRECTIONAL}; /*!< Direction of the stream */

	rxCallback_t rxCallback_;		/*!< RX callback function */
	txCallback_t txCallback_;		/*!< TX callback function */
};

#endif /* SRC_IO_HPP_ */
