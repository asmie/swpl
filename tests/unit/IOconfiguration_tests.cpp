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

 // Check empty configuration objects.
TEST(IOconfiguration, EmptyConfig)
{
	IOconfiguration tst1{};
	IOconfiguration tst2{};



}