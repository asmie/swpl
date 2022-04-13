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
	inline static ConfigurationManager& instance() noexcept
	{
		static ConfigurationManager instance;		// C++11 and above - assures thread-safeness.
		return instance;
	}

	/**
	* Parse configuration passed to command line-like interface.
	* @param[in] argc argument count
	* @param[in] argv pointer to c-like strings containing arguments
	*/
	void parseConfiguration(int argc, char** argv) noexcept;

	/**
	* Get the value from specified key and section - generic version.
	* @param[in] section section to get key from
	* @param[in] key key to find
	* @param[out] value place to store the value
	* @return True if value is valid, otherwise false.
	*/
	template<typename T> bool get(const std::string& section, const std::string& key, T& value) noexcept;

	/**
	* Check if specified key exists under given section.
	* @param[in] section section to get key from
	* @param[in] key key to find
	* @return True if key exists otherwise false.
	*/
	bool settingExists(const std::string& section, const std::string& key) noexcept;

private:
	ConfigurationManager() { }

	std::unique_ptr<ConfigurationFile> configurationFile_;

	ConfigurationStructure configuration_;

};


#endif /* SRC_CONFIGURATIONMANAGER_HPP_ */
