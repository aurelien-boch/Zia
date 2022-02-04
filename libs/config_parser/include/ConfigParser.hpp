#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>

#include <yaml-cpp/yaml.h>

namespace configParser
{
    class ConfigParser
    {
        public:
            ConfigParser(std::string const &path)
            {
                YAML::Node config = YAML::LoadFile(path);
                _config = config;
                if (_config[modules]) {
                    for (auto const &module : _config[modules]) {
                        std::cout ;
                    }
                }
            }
        private:
            YAML::Node _config;
    };
}

#endif // CONFIG_PARSER_HPP