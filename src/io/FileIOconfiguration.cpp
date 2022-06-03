/**
 *  @file   FileIOconfiguration.cpp
 *  @brief  Helper class for FileIO configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.06.03
 *
 */

#include "FileIOconfiguration.hpp"
#include "../config/ConfigurationManager.hpp"

#include <unordered_map>

enum class SettingLabel
{
	FILE,
	EMPTY
};

static const std::unordered_map<SettingLabel, Setting> SETTINGS(
	{
		{SettingLabel::FILE, {"file", SettingType::STRING}},
		{SettingLabel::EMPTY, {"", SettingType::UNKNOWN}}
	});

bool FileIOconfiguration::configure(ConfigurationManager& config, const std::string& section)
{
	bool configurationCorrect = IOconfiguration::configure(config, section);

	config.get(section, SETTINGS.at(SettingLabel::FILE).setting_name, file_);

	if (file_.empty())
		configurationCorrect = false;

	return configurationCorrect;
}
