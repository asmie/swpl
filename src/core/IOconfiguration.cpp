/**
 *  @file   IOconfiguration.cpp
 *  @brief  Helper class for IO configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.05.09
 *
 */

#include "IOconfiguration.hpp"
#include "../config/ConfigurationManager.hpp"

#include <unordered_map>

unsigned int IOconfiguration::last_id_ = 0;

enum class SettingLabel
{
	NAME,
	TYPE,
	DIRECTION,
	BINARY,
	READ_CHUNK_MIN,
	READ_CHUNK_MAX,
	WRITE_CHUNK_MIN,
	WRITE_CHUNK_MAX,
	EMPTY
};

static const std::unordered_map<SettingLabel, Setting> SETTINGS(
{
	{SettingLabel::NAME, {"name", SettingType::STRING}},
	{SettingLabel::TYPE, {"type", SettingType::STRING}},

	{SettingLabel::DIRECTION, {"direction", SettingType::STRING}},
	{SettingLabel::BINARY, {"binary", SettingType::BOOL}},
	{SettingLabel::READ_CHUNK_MIN, {"read_chunk_min", SettingType::INTEGER}},
	{SettingLabel::READ_CHUNK_MAX, {"read_chunk_max", SettingType::INTEGER}},
	{SettingLabel::WRITE_CHUNK_MIN, {"write_chunk_min", SettingType::INTEGER}},
	{SettingLabel::WRITE_CHUNK_MAX, {"write_chunk_max", SettingType::INTEGER}},
	{SettingLabel::EMPTY, {"", SettingType::UNKNOWN}}
});

bool IOconfiguration::configure(ConfigurationManager& config, const std::string& section)
{
	bool configurationCorrect = true;
	std::string direction{};

	config.get(section, SETTINGS.at(SettingLabel::NAME).setting_name, name_);
	config.get(section, SETTINGS.at(SettingLabel::TYPE).setting_name, type_);

	config.get(section, SETTINGS.at(SettingLabel::DIRECTION).setting_name, direction);
	if (direction == "input")
		direction_ = StreamDirection::INPUT;
	else if (direction == "output")
		direction_ = StreamDirection::OUTPUT;

	config.get(section, SETTINGS.at(SettingLabel::BINARY).setting_name, binary_);
	config.get(section, SETTINGS.at(SettingLabel::READ_CHUNK_MIN).setting_name, readChunkMin_);
	config.get(section, SETTINGS.at(SettingLabel::READ_CHUNK_MAX).setting_name, readChunkMax_);
	config.get(section, SETTINGS.at(SettingLabel::WRITE_CHUNK_MIN).setting_name, writeChunkMin_);
	config.get(section, SETTINGS.at(SettingLabel::WRITE_CHUNK_MAX).setting_name, writeChunkMax_);
	
	if (!config.settingExists(section, SETTINGS.at(SettingLabel::NAME).setting_name) ||
		!config.settingExists(section, SETTINGS.at(SettingLabel::TYPE).setting_name))
		configurationCorrect = false;

	if (configurationCorrect)
		id_ = ++last_id_;

	return configurationCorrect;
}
