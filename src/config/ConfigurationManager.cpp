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

}

