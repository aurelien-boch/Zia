#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>

#include <yaml-cpp/yaml.h>

#include <ziapi/Config.hpp>

namespace parser
{
    class ConfigParser
    {
        public:
            ConfigParser(std::string const &path);

            ~ConfigParser() = default;
            
            // doc missing
            [[nodiscard]] inline ziapi::config::Dict const &getConfigMap() const { return (_configMap); }
            
            // doc missing
            void printConfig();

            // doc missing
            [[nodiscard]] ziapi::config::Dict operator[](const std::string &moduleName);

            /*
             * Doc missing
             */
            [[nodiscard]] std::shared_ptr<ziapi::config::Node> getValue(const std::string &root, const std::string &module, const std::string &param);

        private:
            inline void _checkConfig();
            void _putConfigInMap();

            YAML::Node _config;
            ziapi::config::Dict _configMap;
    };
}

#endif // CONFIG_PARSER_HPP