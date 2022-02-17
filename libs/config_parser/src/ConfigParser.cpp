#include "ConfigParser.hpp"

namespace parser
{
    ConfigParser::ConfigParser(std::string const &path) // doc missing
    {
        try {
            _config = YAML::LoadFile(path);
        } catch(const std::exception& e) {
            std::cerr << "[ConfigParser] " << e.what() << std::endl;
        }
        _checkConfig();
        _putConfigInMap();
    }

    void ConfigParser::printConfig()
    {
        for (const auto &[masterKey, masterValue] : _configMap) {
            std::cout << masterKey << ": \n";
            for (const auto &[key, value] : masterValue->AsDict()) {
                std::cout << "\t"<< key << ": \n";
                for (const auto &[parameterKey, parameterValue] : value->AsDict()) {
                    if (parameterValue->index() == ziapi::config::Type::kInt) {
                        std::cout << "\t\t" << parameterKey << ": " << parameterValue->AsInt() << std::endl; 
                        continue;
                    }
                    std::cout << "\t\t" << parameterKey << ": " << parameterValue->AsString() << std::endl;
                }
            }
        }
    }

    ziapi::config::Dict ConfigParser::operator[](const std::string &moduleName)
    {
        if (_configMap["modules"]->AsDict().find(moduleName) == _configMap["modules"]->AsDict().end())
            throw std::range_error("[ConfigParser] not in [modules]");
        return (_configMap["modules"]->AsDict()[moduleName]->AsDict());
    }

    std::shared_ptr<ziapi::config::Node> ConfigParser::getValue(const std::string &root, const std::string &module, const std::string &param) 
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
            throw std::runtime_error("[ConfigParser] Config file not found");
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
                    }
                    // TODO Add a case for sequences
                    tmp[parameters.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(parameters.second.as<std::string>());
                }
                modules[moduleName.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(tmp);
            }
            _configMap[section.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(modules);
        }
    }

}