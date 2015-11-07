#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
public:
    ConfigParser(std::string path);
    ConfigParser(const ConfigParser&) = delete;
    std::string lookup(std::string name);
    void write(std::string name, std::string value);
    void save() const;
private:
    std::string path_;
    std::unordered_map<std::string, std::string> configs_;
    std::vector<std::string> split(std::string data, std::string delim);
};

#endif // CONFIGPARSER_HPP
