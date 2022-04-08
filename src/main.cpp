  /**
 *  @file   main.cpp
 *  @brief  File with entry point.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 */

#include "config/ConfigurationManager.hpp"

int main(int argc, char *argv[])
{
	auto& configurationManager = ConfigurationManager::instance();

	configurationManager.parseConfiguration(argc, argv);

	return 0;
}

