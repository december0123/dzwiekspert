#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <deque>
#include <string>
#include <unordered_map>

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
    void resetToDefault(std::string name);
    std::deque<std::string> split(std::string data, std::string delim);
    void reload();

private:
    std::string path_;
    std::unordered_map<std::string, std::string> configs_;
    std::unordered_map<std::string, std::string> defaultConfigs_;
    void load(std::string path);
};

#endif // CONFIGPARSER_HPP
