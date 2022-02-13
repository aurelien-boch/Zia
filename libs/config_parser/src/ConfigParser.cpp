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
        _putConfigInMap();
    }

    void ConfigParser::_checkConfig()
    {
        if (_config.IsNull())
            throw std::runtime_error("[ConfigParser] Config file not found");
    }

    void ConfigParser::_putConfigInMap()
    {
        for (auto const &section : _config) { // modules
            ziapi::config::Dict modules;
            for (auto const &moduleName : section.second) { // phpcgi, tls
                ziapi::config::Dict tmp;

                for (auto const &parameters : moduleName.second) { // ip : 192, foo : bar
                    if (!parameters.second.IsDefined()) {
                        throw std::runtime_error("[ConfigParser] Parameter undefined");
                    }
                    tmp[parameters.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(parameters.second.as<std::string>());
                }
                modules[moduleName.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(tmp);
            }
            _configMap[section.first.as<std::string>()] = std::make_shared<ziapi::config::Node>(modules);
        }
    }

    void ConfigParser::printConfig()
    {
        for (const auto &[masterKey, masterValue] : _configMap) {
            std::cout << masterKey << ": \n";
            for (const auto &[key, value] : masterValue.get()->AsDict()) {
                std::cout << "\t"<< key << ": \n";
                for (const auto &[parameterKey, parameterValue] : value.get()->AsDict()) {
                    std::cout << "\t\t" << parameterKey << ": " << parameterValue.get()->AsString() << std::endl;
                }
            }
        }
    }
}