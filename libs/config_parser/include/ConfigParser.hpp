#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <ziapi/Config.hpp>

namespace configParser
{
    class ConfigParser
    {
        public:
            ConfigParser(std::string const &path);
            ~ConfigParser() = default;
            std::unordered_map<std::string, std::unordered_map<std::string, std::string>> const &getConfigMap() const;
        private:
            void _checkConfig();
            void _putConfigInMap();
            YAML::Node _config;
            std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _configMap;
    };
}

#endif // CONFIG_PARSER_HPP