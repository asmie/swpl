/**
 *  @file   ConfigurationManager_tests.cpp
 *  @brief  Unit tests for ConfigurationManager.
 *
 *  @author Piotr Olszewski     asmie@asmie.pl
 *
 *  @date   2022.04.13
 *
 * Unit tests for ConfigurationManager.
 *
 */

#include "gtest/gtest.h"
#include "config/ConfigurationManager.hpp"

#include <string>

// Check ConfigurationManager with empty configuration.
TEST(ConfigurationManager, EmptyConfig)
{
	auto& configurationManager = ConfigurationManager::instance();
	bool bTestVal = false;
	int iTestVal = 92;
	long lTestVal = 991;
	double dTestVal = 4.22;
	std::string sTestVal("testStr");

	EXPECT_EQ(false, configurationManager.settingExists("test_section", "test_key"));

	EXPECT_EQ(false, configurationManager.get<bool>("test_section", "test_key", bTestVal));
	EXPECT_EQ(false, bTestVal);		// Important - testVal should not be changed!


}