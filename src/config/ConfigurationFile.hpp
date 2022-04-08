/**
 *  @file   ConfigurationFile.hpp
 *  @brief  Responsible for managing application configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.04.22
 */

#ifndef SRC_CONFIGURATIONFILE_HPP_
#define SRC_CONFIGURATIONFILE_HPP_

#include <string>
#include <map>

 /**
 * Configuration file abstraction.
 */
class ConfigurationFile
{
public:
	/**
	* Default constructor.
	*/
	ConfigurationFile() = default;

	/**
	* Default destructor.
	*/
	virtual ~ConfigurationFile() = default;

	/**
	* Parse specified file and store key=value results in provided map.
	* @param[in] path path to the file with configuration
	* @param[out] settings reference to map where configuration will be stored
	*/
	virtual void parse(std::string path, std::map<std::string, std::string>& settings) = 0;
};



#endif /* SRC_CONFIGURATIONFILE_HPP_ */
