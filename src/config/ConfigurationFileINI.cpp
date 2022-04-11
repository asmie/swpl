/**
 *  @file   ConfigurationFileINI.cpp
 *  @brief  INI reader implementation.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.04.11
 */

#include "ConfigurationFileINI.hpp"

#include <regex>
#include <fstream>

void ConfigurationFileINI::parse(std::string path, ConfigurationStructure& settings)
{
    static const std::regex comment_regex{ R"~(\s*[;#])~" };
    static const std::regex section_regex{ R"~(\s*\[([^\]]+)\])~" };
    static const std::regex value_regex{ R"~(\s*(\S[^ \t=]*)\s*=\s*((\s?\S+)+)\s*$)~" };

    std::ifstream file_reader(path, std::ifstream::in);
    std::string section;
    std::smatch elems;

    for (std::string line; std::getline(file_reader, line);)
    {
        if (line.empty() || std::regex_match(line, elems, comment_regex)) continue;
        
        if (std::regex_match(line, elems, section_regex)) 
        {
            if (elems.size() == 2)
                section = elems[1].str();
        }
        else if (std::regex_match(line, elems, value_regex)) 
        {
            if (elems.size() == 4)
                settings[section][elems[1].str()] = elems[2].str();
        }
    }
}