  /**
 *  @file   main.cpp
 *  @brief  File with entry point.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 */

#include "config/ConfigurationManager.hpp"

#include <memory>

void parseCmdLine(int argc, char **argv);

std::unique_ptr<ConfigurationManager> configurationManager;

int main(int argc, char *argv[])
{
	configurationManager = std::make_unique<ConfigurationManager>();
	parseCmdLine(argc, argv);

	return 0;
}



void parseCmdLine(int argc, char **argv)
{
	if (argc > 2 && argv != nullptr)
	{

	}

}
