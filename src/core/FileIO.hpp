/**
 *  @file   FileIO.hpp
 *  @brief  File input/output .
 *
 *  @author Piotr Olszewski
 *
 *  @date   2022.04.22
 *
 *  This file contains the class for manipulating files.
 */

#ifndef SRC_FILEIO_HPP_
#define SRC_FILEIO_HPP_

#include "IO.hpp"

#include <fstream>
#include <mutex>

/**
* Class for reading files using IO interface.
*/
class FileIO : public IO
{
public:
	FileIO() = default;
	FileIO(std::string fileName, unsigned int id, std::string name, StreamDirection direction) : IO(id, name, direction), fileName_(fileName) { }
	virtual ~FileIO() override;

	/**
	* Demanded configuration:
	* [section_name]
	* type = "file"
	* file = "path to file"
	* 
	* Optional configuration:
	* direction = input/output/bidirectional		# def: bidirectional
	* binary = true/false							# def: false
	* append = true/false							# def: false - only for bi/output
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
	std::string fileName_;
	std::fstream fileStream_;

	std::mutex readLock_;
	std::mutex writeLock_;
};

#endif /* SRC_FILEIO_HPP_ */
