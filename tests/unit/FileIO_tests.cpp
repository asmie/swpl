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

#define TEST_FILE "test_file"
#define TEST_STR "This is the test file"

constexpr const char* conf_test_valid = R"conf(
[io1]
type = file
file = file1
name = file test

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
file = test_file

direction = bidirectional
)conf";

void prepare_file();
void remove_file();

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

	bool result = fio.configure(configurationManager, "io1");

	EXPECT_EQ(result, true);

}


TEST(FileIO, open_close)
{
	auto& configurationManager = ConfigurationManager::instance();
	std::string config(conf);
	configurationManager.parseFromMemory(config);
	FileIO fio;
	bool result = fio.configure(configurationManager, "io1");

	EXPECT_EQ(true, result);
	prepare_file();

	auto res = fio.open();
	EXPECT_GE(res, 0);

	res = fio.close();
	EXPECT_GE(res, 0);
}

TEST(FileIO, read_write)
{
	auto& configurationManager = ConfigurationManager::instance();
	std::string config(conf);
	configurationManager.parseFromMemory(config);
	FileIO fio;
	bool result = fio.configure(configurationManager, "io1");

	EXPECT_EQ(true, result);
	prepare_file();

	auto res = fio.open();
	std::cout << res << std::endl;
	EXPECT_GE(res, 0);

	std::vector<char> buffer;
	buffer.reserve(25);
	auto res2 = fio.read(buffer, 25);

	EXPECT_EQ(res2, 22);

	std::string fRead(buffer.begin(), buffer.end());
	std::string patt(TEST_STR);
	std::string str;

	EXPECT_PRED3([](auto str, auto fRead, auto patt) {
		return str == fRead || str == patt; }, str, fRead, patt);

	result = fio.close();
	EXPECT_GE(res, 0);

	remove_file();
}


#include <fstream>
#include <iostream>
#include <cstdio>
void prepare_file()
{
	std::fstream f(TEST_FILE, std::fstream::out);

	f << TEST_STR << std::endl;

	f.close();
}

void remove_file()
{
	remove(TEST_FILE);
}
