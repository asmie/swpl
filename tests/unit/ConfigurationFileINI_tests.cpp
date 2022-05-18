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
#include "config/ConfigurationFileINI.hpp"

#include <string>
#include <sstream>

 // Check ConfigurationManager with empty configuration.
TEST(ConfigurationFileINI, ParseFromMemory)
{
	constexpr const char* conf = R"conf(
[test1]
key1 = 123
key2 = 312

;valid comment
[test2]
key4 = abcd
key5 = abde asdf zxcv

[test3]
#this is the comment
key1 = true
key2 = false
key1000 = 2.1
)conf";

	std::string config(conf);
	std::stringstream ss;
	ss.str(config);

	ConfigurationFileINI cf;
	ConfigurationFile::ConfigurationStructure cs;

	cf.parse(ss, cs);

	EXPECT_EQ("abcd", cs["test2"]["key4"]);
	EXPECT_EQ("abde asdf zxcv", cs["test2"]["key5"]);

}