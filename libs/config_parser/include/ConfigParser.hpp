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

            /*
             * @brief Parse the config file
             * In cas of error it prints the error
             * @param[in] newPath The path to the config file
             * @throw std::runtime_error if there is an error while parsing the config file
             */
            void parse(std::string const &newPath);
            
            /*
             * @brief Return the config map
             * @return The config map
             */
            [[nodiscard]] inline ziapi::config::Dict const &getConfigMap() const { return (_configMap); }

            /*
             * @brief Overload the [] operator to access the unordered_map of the module
             * It works if the root contains a modules element otherwise it throws
             * @return std::unordered_map<std::string, std::shared_ptr<ziapi::config::Node>> of the module
             */
            [[nodiscard]] ziapi::config::Dict operator[](const std::string &moduleName);

            /*
             * @brief Return the value of a parameter in the module
             * Use As[String, Int, Double, Bool, Dict, Array] to access the right value
             * @param[in] root Root of the config
             * @param[in] module Module name in the root element of the config
             * @param[in] param Parameter in the module int the root element of the config
             * @return A shared pointer to the parameter
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