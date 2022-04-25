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
	/**
	* Default constructor of the class objects.
	*/
	FileIO() = default;

	/**
	* Parametrized constructor that sets the basic data.
	* @param[in] fileName filename with path
	* @param[in] id stream identificator
	* @param[in] name name of the stream
	* @param[in] direction stream direction (in/out/in-out)
	*/
	FileIO(std::string fileName, unsigned int id, std::string name, StreamDirection direction) : IO(id, name, direction), fileName_(fileName) { }
	
	/**
	* Default destructor.
	*/
	virtual ~FileIO() override;

	/**
	* Method allowing module to configure itself using external configuration source.
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
	* @param[in] config reference to the configuration manager facility
	* @param[in] section place where module configuration is stored
	* @return True if configuration is valid, otherwise false.
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

	/**
	* Read at most readMax bytes from the stream and store it in buffer.
	* @param[out] buffer array to store read values
	* @param[in] readMax max bytes to read (must be less than buffer.max_size()).
	* @return Bytes read and stored in the buffer or negative if error occured.
	*/
	virtual ssize_t read(std::vector<char>& buffer, size_t readMax = 0) override;

	/**
	* Write at most writeMax bytes into the stream from the buffer.
	* @param[in] buffer array to be written into the stream
	* @param[in] writeMax max bytes to be written into the stream
	* @return Bytes written to the stream or negative if error occured.
	*/
	virtual ssize_t write(const std::vector<char>& buffer, size_t writeMax = 0) override;

private:
	std::string fileName_;				/*!< Filename (with or without path) */
	std::fstream fileStream_;			/*!< Internal file stream representation */

	std::mutex readLock_;				/*!< Mutex preventing concurrent locking during reading */
	std::mutex writeLock_;				/*!< Mutex preventing concurrent locking during writing */
};

#endif /* SRC_FILEIO_HPP_ */
