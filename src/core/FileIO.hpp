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
	FileIO(std::string fileName, unsigned int id, std::string name, StreamDirection direction) : fileName_(fileName), IO(id, name, direction) { }
	virtual ~FileIO() override;

	virtual bool configure(const ConfigurationManager& config, const std::string section) override;

	virtual int open() override;

	virtual int close() override;

	virtual ssize_t read(std::vector<char>& buffer, size_t readMax = 0) override;

	virtual ssize_t write(const std::vector<char>& buffer, size_t writeMax = 0) override;

private:
	std::string fileName_;
	std::fstream fileStream_;

	std::mutex readLock_;
	std::mutex writeLock_;
};
