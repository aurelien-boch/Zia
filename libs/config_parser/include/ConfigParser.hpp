#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <unordered_map>
#include <initializer_list>
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
            void printConfig();

        private:
            void _checkConfig();
            void _putConfigInMap();
            YAML::Node _config;
            ziapi::config::Dict _configMap;
    };
}

#endif // CONFIG_PARSER_HPP