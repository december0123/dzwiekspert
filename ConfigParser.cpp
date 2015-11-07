#include "ConfigParser.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

ConfigParser::ConfigParser(std::string path)
    : path_{path}
{
    std::ifstream file{path};
    for(std::string line; std::getline(file, line);)
    {
        auto c = split(line, "=");
        if (c.size() != 2)
        {
            throw std::logic_error{"CONFIG FILE MALFORMED"};
        }
        configs_[c[0]] = c[1];
    }
}

std::string ConfigParser::lookup(std::string name)
{
    return configs_[name];
}

void ConfigParser::write(std::string name, std::string value)
{
    configs_[name] = value;
}

void ConfigParser::save() const
{
    std::ofstream file{path_};
    for(const auto& conf : configs_)
    {
        file << conf.first << "=" << conf.second << std::endl;
    }
}

std::vector<std::string> ConfigParser::split(std::string data, std::string delim)
{
    std::vector<std::string> output;
    auto pos = std::string::npos;
    do
    {
        pos = data.find(delim);
        output.push_back(data.substr(0, pos));
        if (std::string::npos != pos)
            data = data.substr(pos + delim.size());
    } while (std::string::npos != pos);
    return output;
}
