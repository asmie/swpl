/**
 *  @file    ConfigurationManager.hpp
 *  @brief  Responsible for managing application configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 */

#ifndef SRC_CONFIGURATIONMANAGER_HPP_
#define SRC_CONFIGURATIONMANAGER_HPP_

#include "ConfigurationFile.hpp"

#include <string>
#include <memory>
#include <unordered_map>

#include <cfloat>
#include <climits>

/**
* Configuration manager class that is responsible for handling application settings. Parses
* command line arguments and if needed manages parsing file with configuration.
* Implements singleton pattern as there is always only one, global configuration. 
*/
class ConfigurationManager
{
	typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ConfigurationStructure;

public:
	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;
	~ConfigurationManager() { }

	/**
	* Gets reference to configuration manager instance.
	* @return ConfigurationManager instance reference.
	*/
	inline static ConfigurationManager& instance()
	{
		static ConfigurationManager instance;		// C++11 and above - assures thread-safeness.
		return instance;
	}

	/**
	* Parse configuration passed to command line-like interface.
	* @param[in] argc argument count
	* @param[in] argv pointer to c-like strings containing arguments
	*/
	void parseConfiguration(int argc, char** argv);

	/**
	* Get value of the specified key in specified section.
	* @param[in] section section where key is stored
	* @param[in] key key to look for
	*/
	template<typename T> T get(const std::string& section, const std::string& key);

private:
	ConfigurationManager() { }

	std::unique_ptr<ConfigurationFile> configurationFile_;

	ConfigurationStructure configuration_;

};


#endif /* SRC_CONFIGURATIONMANAGER_HPP_ */
