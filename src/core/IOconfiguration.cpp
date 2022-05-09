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

constexpr std::string_view SETT_DIRECTION = "direction";
constexpr std::string_view SETT_BINARY = "binary";
constexpr std::string_view SETT_APPEND = "append";
constexpr std::string_view SETT_R_CHUNK = "read_chunk";
constexpr std::string_view SETT_W_CHUNK = "write_chunk";

bool IOconfiguration::configure(ConfigurationManager& config, const std::string& section)
{
	bool configurationCorrect = false;

	config.get(section, "name", name_);

	return configurationCorrect;
}