/**
 *  @file    ConfigurationManager.cpp
 *  @brief  Responsible for managing application configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 */


#include "ConfigurationManager.hpp"
#include "ConfigurationFileINI.hpp"

#include <fstream>
#include <regex>

long long convertToLongLong(const std::string& value);

void ConfigurationManager::parseConfiguration(int argc, char** argv)
{
	bool readFromFile = false;
	const char* fileToRead = nullptr;

	if (argv == nullptr)
		return;																	

	for (auto i = 0; i < argc; i++)
	{
		
	}

	if (readFromFile)
	{
		configurationFile_ = std::make_unique<ConfigurationFileINI>();
		configurationFile_->parse(fileToRead, configuration_);
	}
}


template<typename T> 
T ConfigurationManager::get(const std::string& section, const std::string& key)
{
	throw std::runtime_error("invalid conversion");
}


bool ConfigurationManager::settingExists(const std::string& section, const std::string& key)
{
	return configuration_.find(section) != configuration_.end() && configuration_[section].find(key) != configuration_[section].end();
}


/**
* Get the value from specified key and section then convert to bool.
* @param[in] section section to get key from
* @param[in] key key to find
* @return The requested value or 0 if there is no valid conversion.
*/
template<>
int ConfigurationManager::get(const std::string& section, const std::string& key)
{
	long long result = 0;

	try 
	{
		if (settingExists(section, key))
			result = convertToLongLong(configuration_[section][key]);
	}
	catch (std::runtime_error &e)
	{

	}

	return result;
}

/**
* Get the value from specified key and section then convert to int.
* @param[in] section section to get key from
* @param[in] key key to find
* @return The requested value.
*/
template<>
long ConfigurationManager::get(const std::string& section, const std::string& key)
{
	return configuration_[section][key] == "true" || configuration_[section][key] == "1";
}

/**
* Get the value from specified key and section then convert to bool.
* @param[in] section section to get key from
* @param[in] key key to find
* @return The requested value.
*/
template<>
float ConfigurationManager::get(const std::string& section, const std::string& key)
{
	return configuration_[section][key] == "true" || configuration_[section][key] == "1";
}


/**
 * Convert std::string to long long.
 *
 * @param[in] value value to be converted
 * @return Long long based on the converted value. 0 if error.
 */
long long convertToLongLong(const std::string& value)
{
	std::smatch matcher;
	std::regex  numberRegex(R"~(^\s*(\-?\d+)\s*$)~");

	if (std::regex_match(value, matcher, numberRegex))
		return std::strtoll(matcher[1].str().c_str(), nullptr, 10);
	else
		throw std::runtime_error("bad conversion");
}
