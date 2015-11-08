#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
public:
    ConfigParser(std::string path);
    ConfigParser(const ConfigParser&) = default;
    ConfigParser(ConfigParser&&) = default;
    ConfigParser& operator=(const ConfigParser&) = default;
    ConfigParser& operator=(ConfigParser&&) = default;
    std::string lookup(std::string name);
    void write(std::string name, std::string value);
    void save() const;
    std::vector<std::string> split(std::string data, std::string delim);

private:
    std::string path_;
    std::unordered_map<std::string, std::string> configs_;
    std::unordered_map<std::string, std::string> defaultConfigs_;
};

#endif // CONFIGPARSER_HPP
