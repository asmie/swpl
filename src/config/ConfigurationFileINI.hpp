/**
 *  @file   ConfigurationFile.hpp
 *  @brief  Responsible for managing application configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.04.22
 */

#ifndef SRC_CONFIGURATIONFILEINI_HPP_
#define SRC_CONFIGURATIONFILEINI_HPP_

#include "ConfigurationFile.hpp"

#include <string>
#include <unordered_map>

 /**
 * Configuration stored in INI file.
 */
class ConfigurationFileINI : public ConfigurationFile
{
public:
	/**
	* Default constructor.
	*/
	ConfigurationFileINI() = default;

	/**
	* Default destructor.
	*/
	virtual ~ConfigurationFileINI() = default;

	/**
	* Parse specified file and store key=value results in provided map.
	* @param[in] path path to the file with configuration
	* @param[out] settings reference to map where configuration will be stored
	*/
	void parse(std::string path, ConfigurationStructure& settings) override;

	/**
	* Parse specified memory location and store key=value results in provided map.
	* @param[in] config string stream with configuration
	* @param[out] settings reference to map where configuration will be stored
	*/
	void parse(std::stringstream& config, ConfigurationStructure& settings) override;
};

#endif /* SRC_CONFIGURATIONFILE_HPP_ */
