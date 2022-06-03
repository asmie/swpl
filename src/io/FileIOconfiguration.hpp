/**
 *  @file   FileIOconfiguration.hpp
 *  @brief  Helper class for FileIO configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.06.03
 *
 */

#ifndef SRC_FILEIOCONFIGURATION_HPP_
#define SRC_FILEIOCONFIGURATION_HPP_

#include "Global.h"
#include "core/IOconfiguration.hpp"

#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>


/**
* Configuration implementation for FileIO class.
*/
class FileIOconfiguration : public IOconfiguration
{
public:
	/**
	* Default constructor of the class objects.
	*/
	FileIOconfiguration() = default;

	/**
	* Default destructor.
	*/
	virtual ~FileIOconfiguration() = default;

	/**
	* Method allowing module to configure itself using external configuration source.
	* Supported configuration:
	* [section_name]
	* file = "file path"
	* @param[in] config reference to the configuration manager facility
	* @param[in] section place where module configuration is stored
	* @return True if configuration is valid, otherwise false.
	*/
	virtual bool configure(ConfigurationManager& config, const std::string& section) override;

	/**
	* Get file name or path.
	* @return std::string with file.
	*/
	virtual std::string getFile() const
	{
		return file_;
	}

private:
	std::string file_{ "" };										/*!< Path to the file */
};

#endif /* SRC_FILEIOCONFIGURATION_HPP_ */
