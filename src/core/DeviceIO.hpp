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

#include "IO.hpp"

#include <cstdlib>
#include <mutex>

 /**
 * Class for reading files using IO interface.
 */
class DeviceIO : public IO
{
public:
	DeviceIO() = default;
	DeviceIO(std::string devicePath, unsigned int id, std::string name, StreamDirection direction) : devicePath_(devicePath), devFd_(-1), IO(id, name, direction) { }
	virtual ~DeviceIO() override;

	/**
	* Demanded configuration:
	* [section_name]
	* type = "device"
	* file = "path to device"
	*
	* Optional configuration:
	* direction = input/output/bidirectional		# def: bidirectional
	* binary = true/false							# def: false
	* read_chunk = 128								# read chunks - only for bi/input
	* write_chunk = 128								# write chunks - onlu for bi/output
	*/
	virtual bool configure(const ConfigurationManager& config, const std::string section) override;

	/**
	* Opens file. File must be opened before any operation and remains open util close() call.
	* @return 0 if successful, otherwise negative info with error code.
	*/
	virtual int open() override;

	/**
	* Closes file. Any operation after close is not considered valid.
	* @return 0 if successful, otherwise negative info with error code.
	*/
	virtual int close() override;

	virtual ssize_t read(std::vector<char>& buffer, size_t readMax = 0) override;

	virtual ssize_t write(const std::vector<char>& buffer, size_t writeMax = 0) override;

private:
	std::string devicePath_;
	int devFd_;

	std::mutex readLock_;
	std::mutex writeLock_;
};
