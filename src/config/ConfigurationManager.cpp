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

 /**
  * Convert std::string to long long.
  *
  * @param[in] value value to be converted
  * @return Long long based on the converted value. Exception or 0 is returned if error.
  */
long long convertToLongLong(const std::string& value);

/**
 * Convert std::string to long long.
 *
 * @param[in] value value to be converted
 * @return Long long based on the converted value. 0 if error.
 */
double convertToDouble(const std::string& value);


void ConfigurationManager::parseConfiguration(int argc, char** argv) noexcept
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
bool ConfigurationManager::get(const std::string& section, const std::string& key, T& value) noexcept
{
	return false;
}


bool ConfigurationManager::settingExists(const std::string& section, const std::string& key) noexcept
{
	bool exists = true;

	try 
	{
		configuration_.at(section).at(key);
	}
	catch (std::out_of_range&) 
	{
		exists = false;
	}

	return exists;
}


/**
* Get the value from specified key and section - bool version.
* @param[in] section section to get key from
* @param[in] key key to find
* @param[out] value place to store the value
* @return True if value is valid, otherwise false.
*/
template<>
bool ConfigurationManager::get(const std::string& section, const std::string& key, bool& value) noexcept
{
	bool result = false;

	if (settingExists(section, key))
	{
		std::string testVal = configuration_[section][key];

		std::transform(testVal.begin(), testVal.end(), testVal.begin(),
			[](unsigned char c) { return std::tolower(c); });

		value = (testVal == "true" || testVal == "1");
		result = true;
	}

	return result;
}

/**
* Get the value from specified key and section - int version.
* @param[in] section section to get key from
* @param[in] key key to find
* @param[out] value place to store the value
* @return True if value is valid, otherwise false.
*/
template<>
bool ConfigurationManager::get(const std::string& section, const std::string& key, int& value) noexcept
{
	bool result = false;

	try
	{
		if (settingExists(section, key))
		{
			value = static_cast<int>(convertToLongLong(configuration_[section][key]));
			result = true;
		}
	}
	catch (std::runtime_error&) {}

	return result;
}

/**
* Get the value from specified key and section - long version.
* @param[in] section section to get key from
* @param[in] key key to find
* @param[out] value place to store the value
* @return True if value is valid, otherwise false.
*/
template<>
bool ConfigurationManager::get(const std::string& section, const std::string& key, long& value) noexcept
{
	bool result = false;

	try
	{
		if (settingExists(section, key))
		{
			value = static_cast<long>(convertToLongLong(configuration_[section][key]));
			result = true;
		}
	}
	catch (std::runtime_error&) { }

	return result;
}

/**
* Get the value from specified key and section - double version.
* @param[in] section section to get key from
* @param[in] key key to find
* @param[out] value place to store the value
* @return True if value is valid, otherwise false.
*/
template<>
bool ConfigurationManager::get(const std::string& section, const std::string& key, double& value) noexcept
{
	bool result = false;

	try
	{
		if (settingExists(section, key))
		{
			value = convertToDouble(configuration_[section][key]);
			result = true;
		}
	}
	catch (std::runtime_error&) {}

	return result;
}



long long convertToLongLong(const std::string& value)
{
	std::smatch matcher;
	std::regex  numberRegex(R"~(^\s*(\-?\d+)\s*$)~");

	if (std::regex_match(value, matcher, numberRegex))
		return std::strtoll(matcher[1].str().c_str(), nullptr, 10);
	else
		throw std::runtime_error("bad conversion");
}


double convertToDouble(const std::string& value)
{
	std::smatch matcher;
	std::regex  numberRegex(R"~(^\s*([+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?))\s*$)~");;

	if (std::regex_match(value, matcher, numberRegex))
		return std::strtold(matcher[1].str().c_str(), nullptr);
	else
		throw std::runtime_error("bad conversion");
}