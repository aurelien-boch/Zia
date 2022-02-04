#include "ConfigParser.hpp"

namespace configParser
{

    ConfigParser::ConfigParser(std::string const &path)
    {
        try {
            _config = YAML::LoadFile(path);
        } catch(const std::exception& e) {
            std::cerr << "[ConfigParser] " << e.what() << '\n';
        }
        _checkConfig();
    }

    void ConfigParser::_checkConfig()
    {
        if (_config.IsNull())
            throw std::runtime_error("[ConfigParser] Config file not found");
    }

    void ConfigParser::_putConfigInMap()
    {
        for (auto const &section : _config)
        {
            std::unordered_map<std::string, std::string> sectionMap;
            for (auto const &key : section.second)
            {
                sectionMap[key.first.as<std::string>()] = key.second.as<std::string>();
            }
            _configMap[section.first.as<std::string>()] = sectionMap;
        }
    }

}