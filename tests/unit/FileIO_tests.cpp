/**
 *  @file   FileIO_tests.cpp
 *  @brief  Unit tests for FileIO.
 *
 *  @author Piotr Olszewski     asmie@asmie.pl
 *
 *  @date   2022.06.01
 *
 */


#include "gtest/gtest.h"
#include "io/FileIO.hpp"
#include "config/ConfigurationManager.hpp"

#include <string>
#include <sstream>

constexpr const char* conf_test_valid = R"conf(
[io1]
type = file
file = file1

direction = bidirectional
binary = true
append = true
read_chunk_min = 10
read_chunk_max = 130
write_chunk_min = 98
write_chunk_max = 223	
)conf";

constexpr const char* conf = R"conf(
[io1]
type = file
file = file1

direction = bidirectional
)conf";

TEST(FileIO, configure)
{
	auto& configurationManager = ConfigurationManager::instance();
	std::string config(conf_test_valid);

	configurationManager.parseFromMemory(config);

	bool bTestVal = false;
	int iTestVal = 92;
	std::string sTestVal("testStr");

	EXPECT_EQ(true, configurationManager.get<std::string>("io1", "type", sTestVal));
	EXPECT_EQ("file", sTestVal);

	EXPECT_EQ(true, configurationManager.get<std::string>("io1", "file", sTestVal));
	EXPECT_EQ("file1", sTestVal);

	EXPECT_EQ(true, configurationManager.get<std::string>("io1", "direction", sTestVal));
	EXPECT_EQ("bidirectional", sTestVal);

	EXPECT_EQ(true, configurationManager.get<bool>("io1", "binary", bTestVal));
	EXPECT_EQ(true, bTestVal);

	EXPECT_EQ(true, configurationManager.get<bool>("io1", "append", bTestVal));
	EXPECT_EQ(true, bTestVal);

	EXPECT_EQ(true, configurationManager.get<int>("io1", "read_chunk_min", iTestVal));
	EXPECT_EQ(10, iTestVal);

	EXPECT_EQ(true, configurationManager.get<int>("io1", "read_chunk_max", iTestVal));
	EXPECT_EQ(130, iTestVal);

	EXPECT_EQ(true, configurationManager.get<int>("io1", "write_chunk_min", iTestVal));
	EXPECT_EQ(98, iTestVal);

	EXPECT_EQ(true, configurationManager.get<int>("io1", "write_chunk_max", iTestVal));
	EXPECT_EQ(223, iTestVal);

	FileIO fio;

	fio.configure(configurationManager, "io1");
}


TEST(FileIO, open)
{


}


TEST(FileIO, close)
{


}


TEST(FileIO, read)
{


}


TEST(FileIO, write)
{


}