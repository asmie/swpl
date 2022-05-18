/**
 *  @file   Configurable.hpp
 *  @brief  Interface for configurable objects..
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.05.09
 */

#ifndef SRC_CORE_CONFIGURABLE_HPP_
#define SRC_CORE_CONFIGURABLE_HPP_

#include <string>

class ConfigurationManager;

enum class SettingType
{
	UNKNOWN,
	STRING,
	INTEGER,
	DOUBLE,
	BOOL
};

typedef struct 
{
	std::string setting_name;
	SettingType setting_type;
} Setting;

/**
* Interface for all classes that can be configured using the standard application configuration file.
* It depends on the fact that each class should know its configuration section and is able to parse
* any fields that can be placed under that section.
*/
class Configurable
{
public:
	/**
	* Method allowing module to configure itself using external configuration source.
	* @param[in] config reference to the configuration manager facility
	* @param[in] section place where module configuration is stored
	* @return True if configuration is valid, otherwise false.
	*/
	virtual bool configure(ConfigurationManager& config, const std::string& section) = 0;
};

#endif /* SRC_CORE_CONFIGURABLE_HPP_ */
