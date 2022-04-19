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

	EXPECT_EQ(false, configurationManager.get<int>("test_section", "test_key", iTestVal));
	EXPECT_EQ(92, iTestVal);

	EXPECT_EQ(false, configurationManager.get<long>("test_section", "test_key", lTestVal));
	EXPECT_EQ(991, lTestVal);

	EXPECT_EQ(false, configurationManager.get<double>("test_section", "test_key", dTestVal));
	EXPECT_EQ(4.22, dTestVal);

	EXPECT_EQ(false, configurationManager.get<std::string>("test_section", "test_key", sTestVal));
	EXPECT_EQ("testStr", sTestVal);
}


long long convertToLongLong(const std::string& value);
double convertToDouble(const std::string& value);

TEST(ConfigurationManagerHelpers, convertToDoubleTestCorrect)
{
	std::string t1("1"), t2("1.0"), t3("0.0"), t4("999.99112"), t5("-10.233"), t6("10e3"), t7("1.0e-10");

	auto w1 = convertToDouble(t1);
	EXPECT_EQ(1.0, w1);

	auto w2 = convertToDouble(t2);
	EXPECT_EQ(1.0, w2);

	auto w3 = convertToDouble(t3);
	EXPECT_EQ(0.0, w3);

	auto w4 = convertToDouble(t4);
	EXPECT_EQ(999.99112, w4);

	auto w5 = convertToDouble(t5);
	EXPECT_EQ(-10.233, w5);

	auto w6 = convertToDouble(t6);
	EXPECT_EQ(10e3, w6);

	auto w7 = convertToDouble(t7);
	EXPECT_EQ(1.0e-10, w7);
}

TEST(ConfigurationManagerHelpers, convertToDoubleTestIncorrect)
{
	std::string t1(""), t2("test"), t3("   328uu7730   ");

	EXPECT_THROW({
		auto w1 = convertToDouble(t1);
		}, std::runtime_error);

	EXPECT_THROW({
		auto w2 = convertToDouble(t2);
		}, std::runtime_error);

	EXPECT_THROW({
		auto w3 = convertToDouble(t3);
		}, std::runtime_error);
}

TEST(ConfigurationManagerHelpers, convertToLongLongTestCorrect)
{
	std::string t1("0"), t2("-1293821"), t3("     394938283"), t4("00002939"), t5("+199234"), t6("9993993"), t7("-0");

	auto w1 = convertToLongLong(t1);
	EXPECT_EQ(0, w1);

	auto w2 = convertToLongLong(t2);
	EXPECT_EQ(-1293821, w2);

	auto w3 = convertToLongLong(t3);
	EXPECT_EQ(394938283, w3);

	auto w4 = convertToLongLong(t4);
	EXPECT_EQ(2939, w4);

	auto w5 = convertToLongLong(t5);
	EXPECT_EQ(199234, w5);

	auto w6 = convertToLongLong(t6);
	EXPECT_EQ(9993993, w6);

	auto w7 = convertToLongLong(t7);
	EXPECT_EQ(0, w7);
}

TEST(ConfigurationManagerHelpers, convertToLongLongTestIncorrect)
{
	std::string t1(""), t2("test"), t3("   328uu7730   ");

	EXPECT_THROW({
		auto w1 = convertToLongLong(t1);
		}, std::runtime_error);

	EXPECT_THROW({
		auto w2 = convertToLongLong(t2);
		}, std::runtime_error);

	EXPECT_THROW({
		auto w3 = convertToLongLong(t3);
		}, std::runtime_error);
}