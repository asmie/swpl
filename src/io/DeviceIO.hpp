/**
 *  @file   DeviceIO.hpp
 *  @brief  Device input/output .
 *
 *  @author Piotr Olszewski
 *
 *  @date   2022.04.22
 *
 *  This file contains the class for manipulating devices.
 */

#ifndef SRC_DEVICEIO_HPP_
#define SRC_DEVICEIO_HPP_

#include "core/IO.hpp"

#include <cstdlib>
#include <mutex>

 /**
 * Class for reading files using IO interface.
 */
class DeviceIO : public IO
{
public:
	/**
	* Default constructor of the class objects.
	*/
	DeviceIO() = default;

	/**
	* Parametrized constructor that sets the basic data.
	* @param[in] devicePath path to the device
	* @param[in] id stream identificator
	* @param[in] name name of the stream
	* @param[in] direction stream direction (in/out/in-out)
	*/
	DeviceIO(const std::string& devicePath, unsigned int id, const std::string& name, StreamDirection direction) : IO(id, name, direction), devicePath_(devicePath), devFd_(-1) { }
	
	/**
	* Object destructor.
	*/
	virtual ~DeviceIO() override;

	/**
	* Method allowing module to configure itself using external configuration source.
	* Demanded configuration:
	* [section_name]
	* type = "device"
	* file = "path to device"
	*
	* Optional configuration:
	* direction = input/output/bidirectional		# def: bidirectional
	* binary = true/false							# def: false
	* read_chunk_min = 0							# read chunks - only for bi/input
	* read_chunk_max = 128							# read chunks - only for bi/input
	* write_chunk_min = 0							# write chunks - onlu for bi/output
	* write_chunk_max = 128							# write chunks - onlu for bi/output
	* @param[in] config reference to the configuration manager facility
	* @param[in] section place where module configuration is stored
	* @return True if configuration is valid, otherwise false.
	*/
	virtual bool configure(ConfigurationManager& config, const std::string& section) override;

	/**
	* Opens device. Device must be opened before any operation and remains open util close() call.
	* @return 0 if successful, otherwise negative info with error code.
	*/
	virtual int open() override;

	/**
	* Closes device. Any operation after close is not considered valid.
	* @return 0 if successful, otherwise negative info with error code.
	*/
	virtual int close() override;

	/**
	* Read at most readMax bytes from the device and store it in buffer.
	* @param[out] buffer array to store read values
	* @param[in] readMax max bytes to read (must be less than buffer.max_size()).
	* @return Bytes read and stored in the buffer or negative if error occured.
	*/
	virtual ssize_t read(std::vector<char>& buffer, size_t readMax = 0) override;

	/**
	* Write at most writeMax bytes into the device from the buffer.
	* @param[in] buffer array to be written into the stream
	* @param[in] writeMax max bytes to be written into the stream
	* @return Bytes written to the stream or negative if error occured.
	*/
	virtual ssize_t write(const std::vector<char>& buffer, size_t writeMax = 0) override;

private:
	std::string devicePath_;			/*!< Device path */
	int devFd_;							/*!< Internal representation of file descriptor */

	std::mutex readLock_;				/*!< Mutex preventing concurrent locking during reading */
	std::mutex writeLock_;				/*!< Mutex preventing concurrent locking during writing */
};

#endif /* SRC_DEVICEIO_HPP_ */