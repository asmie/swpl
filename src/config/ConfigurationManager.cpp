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
#include <string>
#include <sstream>

#include <cfloat>
#include <climits>

 /**
  * Convert std::string to long long.
  *
  * @param[in] value value to be converted
  * @return Long long based on the converted value. Exception is returned if error.
  */
long long convertToLongLong(const std::string& value);

/**
 * Convert std::string to double.
 *
 * @param[in] value value to be converted
 * @return Long long based on the converted value. Exception is returned if error.
 */
double convertToDouble(const std::string& value);


void ConfigurationManager::parseConfiguration(int argc, char** argv) noexcept
{
	if (argv == nullptr)
		return;

	appConfig_.config = nullptr;
	appConfig_.daemonize = true;
	appConfig_.help = false;
	appConfig_.valid = false;
	appConfig_.verbose = false;

	for (auto i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'c':
					appConfig_.config = argv[i+1];
					appConfig_.valid = true;
					i++;
					break;
				case 'v':
					appConfig_.verbose = true;
					break;
				case 'h':
					appConfig_.help = true;
					appConfig_.valid = true;
					break;
				case 'd':
					appConfig_.daemonize = false;
					break;
				default:
					appConfig_.valid = false;
			}
		}
	}

	if (appConfig_.valid == true && appConfig_.config != nullptr)
	{
		configurationFile_ = std::make_unique<ConfigurationFileINI>();
		configurationFile_->parse(appConfig_.config, configuration_);
	}
}

void ConfigurationManager::parseFromMemory(std::string& configuration) noexcept
{
	std::stringstream ss;
	ss.str(configuration);

	configurationFile_ = std::make_unique<ConfigurationFileINI>();
	configurationFile_->parse(ss, configuration_);
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
		auto& test = configuration_.at(section).at(key);
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
* Get the value from specified key and section - long version.
* @param[in] section section to get key from
* @param[in] key key to find
* @param[out] value place to store the value
* @return True if value is valid, otherwise false.
*/
template<>
bool ConfigurationManager::get(const std::string& section, const std::string& key, size_t& value) noexcept
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
	catch (std::runtime_error&) {}

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

/**
* Get the value from specified key and section - std::string version.
* @param[in] section section to get key from
* @param[in] key key to find
* @param[out] value place to store the value
* @return True if value is valid, otherwise false.
*/
template<>
bool ConfigurationManager::get(const std::string& section, const std::string& key, std::string& value) noexcept
{
	bool result = false;

	if (settingExists(section, key))
	{
		value = configuration_[section][key];
		result = true;
	}

	return result;
}


long long convertToLongLong(const std::string& value)
{
	std::smatch matcher;
	std::regex  numberRegex(R"~(^\s*([+-]?\d+)\s*$)~");

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
		return static_cast<double>(std::strtold(matcher[1].str().c_str(), nullptr));
	else
		throw std::runtime_error("bad conversion");
}
