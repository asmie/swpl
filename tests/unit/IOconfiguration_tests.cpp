/**
 *  @file   IOconfiguration_tests.cpp
 *  @brief  Unit tests for IO configuration abstraction.
 *
 *  @author Piotr Olszewski     asmie@asmie.pl
 *
 *  @date   2022.05.10
 *
 */

#include "gtest/gtest.h"
#include "core/IOconfiguration.hpp"
#include "config/ConfigurationManager.hpp"

#include <string>

constexpr const char* conf = R"conf(
[test1]
type = IO
name = Stream1

direction = input
binary = false
read_chunk_min = 120
read_chunk_max = 250
write_chunk_min = 100
write_chunk_max = 200
)conf";

 // Check empty configuration objects.
TEST(IOconfiguration, EmptyConfig)
{
	IOconfiguration tst1{};
	IOconfiguration tst2{};

}

TEST(IOconfiguration, ValidConfig)
{
	auto& cm = ConfigurationManager::instance();
	std::string cfg(conf);
	cm.parseFromMemory(cfg);

	IOconfiguration io;

	bool test = io.configure(cm, "test1");

	EXPECT_EQ(true, test);

	EXPECT_EQ("IO", io.getType());
	EXPECT_EQ("Stream1", io.getName());
	EXPECT_EQ(StreamDirection::INPUT, io.getDirection());
	EXPECT_EQ(false, io.getBinary());
	EXPECT_EQ(120, io.getReadChunkMin());
	EXPECT_EQ(250, io.getReadChunkMax());
	EXPECT_EQ(100, io.getWriteChunkMin());
	EXPECT_EQ(200, io.getWriteChunkMax());
}


TEST(IOconfiguration, CheckCopying)
{
	auto& cm = ConfigurationManager::instance();
	std::string cfg(conf);
	cm.parseFromMemory(cfg);

	IOconfiguration io;

	bool test = io.configure(cm, "test1");

	IOconfiguration copy_io(io);

	EXPECT_EQ(true, copy_io == io);
}