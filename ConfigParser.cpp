#include "ConfigParser.hpp"

#include <stdexcept>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>

ConfigParser::ConfigParser(std::string path)
    : path_{path}, defaultConfigs_{{"hand", "right"}, {"tuning", "E2,A3,D3,G3,B3,E4"}, {"basic", "440"}}
{
    std::ifstream file(path, std::ios::in);
    if (file.good())
    {
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
    else
    {
        std::ofstream file{path};
        for(const auto& option : defaultConfigs_)
        {
            file << option.first << "=" << option.second << std::endl;
        }
        configs_ = defaultConfigs_;
    }
}

std::string ConfigParser::lookup(std::string name)
{
    auto val = configs_.find(name);
    if (val != configs_.end())
    {
        return val->second;
    }
    throw std::logic_error{"OPTION NOT FOUND"};
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

std::deque<std::string> ConfigParser::split(std::string data, std::string delim)
{
    std::deque<std::string> output;
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
