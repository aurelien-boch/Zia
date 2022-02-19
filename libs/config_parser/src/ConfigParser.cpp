#include "ConfigParser.hpp"

namespace parser
{
    ConfigParser::ConfigParser(std::string const &path)
    {
        parse(path);
    }

    void ConfigParser::parse(std::string const &newPath)
    {
        try {
            _config = YAML::LoadFile(newPath);
        } catch(YAML::BadFile const &e) {
            throw std::runtime_error("[ConfigParser] Error while parsing the config file");
        }
        _checkConfig();
        _putConfigInMap();
    }

    ziapi::config::Dict ConfigParser::operator[](std::string const &moduleName)
    {
        if (_configMap["modules"]->AsDict().find(moduleName) == _configMap["modules"]->AsDict().end())
            throw std::range_error("[ConfigParser] not in [modules]");
        return (_configMap["modules"]->AsDict()[moduleName]->AsDict());
    }

    std::shared_ptr<ziapi::config::Node> ConfigParser::getValue(std::string const &root, std::string const &module, std::string const &param) 
    {
        if (_configMap.find(root) == _configMap.end())
            throw std::range_error("[ConfigParser] not in config");
        if (_configMap[root]->AsDict().find(module) == _configMap[root]->AsDict().end())
            throw std::range_error("[ConfigParser] not in [" + root + "]");
        if (_configMap[root]->AsDict()[module]->AsDict().find(param) == _configMap[root]->AsDict()[module]->AsDict().end())
            throw std::range_error("[ConfigParser] not in [" + root + "][" + module + "]");
        return (_configMap[root]->AsDict()[module]->AsDict()[param]);
    }

    inline void ConfigParser::_checkConfig()
    {
        if (_config.IsNull())
            throw std::runtime_error("[ConfigParser] Config is null");
    }

    void ConfigParser::_putConfigInMap()
    {
        for (auto const &section : _config) {
            ziapi::config::Dict modules;

            for (auto const &moduleName : section.second) {
                ziapi::config::Dict tmp;

                for (auto const &parameters : moduleName.second) {
                    if (!parameters.second.IsDefined())
                        throw std::runtime_error("[ConfigParser] Parameter undefined");
                    try {
                        tmp[parameters.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(parameters.second.as<int>());
                        continue;
                    } catch(const YAML::BadConversion &e) {
                        tmp[parameters.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(parameters.second.as<std::string>());
                    }
                }
                modules[moduleName.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(tmp);
            }
            _configMap[section.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(modules);
        }
    }

}