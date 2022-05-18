/**
 *  @file   ConfigurationFile.hpp
 *  @brief  Interface for configuration files.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.04.22
 */

#ifndef SRC_CONFIGURATIONFILE_HPP_
#define SRC_CONFIGURATIONFILE_HPP_

#include <string>
#include <sstream>
#include <unordered_map>

 /**
 * Configuration file abstraction.
 */
class ConfigurationFile
{
public:
	typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ConfigurationStructure;

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
	virtual void parse(std::string path, ConfigurationStructure& settings) = 0;

	/**
	* Parse specified memory location and store key=value results in provided map.
	* @param[in] config string stream with configuration
	* @param[out] settings reference to map where configuration will be stored
	*/
	virtual void parse(std::stringstream& config, ConfigurationStructure& settings) = 0;
};

#endif /* SRC_CONFIGURATIONFILE_HPP_ */
