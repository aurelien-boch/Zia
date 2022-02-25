#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <ziapi/Config.hpp>
#include <Loader.hpp>

#ifdef _WIN32
    #define DllExport __declspec(dllexport)
#else
    #define DllExport
#endif

namespace parser
{
    struct LibInfo {
        std::string name;
        std::string configPath;
        std::string modulePath;
    };

    class DllExport ConfigParser
    {
        public:
            explicit ConfigParser(std::string const &path);

            ~ConfigParser() = default;

            /*
             * @brief Parses the config file
             * In case of error it prints the error
             * @param[in] newPath The path to the config file
             * @throw std::runtime_error if there is an error while parsing the config file
             */
            void parse(std::string const &newPath);

            /*
             * @brief Returns the config map
             * @return The config map
             */
            [[nodiscard]] inline ziapi::config::Dict const &getConfigMap() const { return (_configMap); }

            /*
             * @brief Overloads the [] operator to access the unordered_map of the module
             * @throw std::range_error if the module is not in the config
             * @return std::unordered_map<std::string, std::shared_ptr<ziapi::config::Node>> of the module
             */
            [[nodiscard]] ziapi::config::Dict operator[](std::string const &moduleName) const;

            /*
             * @brief Returns the value of a parameter in the module
             * Use As[String, Int, Double, Bool, Dict, Array] to access the right value
             * @param[in] root Root of the config
             * @param[in] module Module name in the root element of the config
             * @param[in] param Parameter in the module int the root element of the config
             * @return A shared pointer to the parameter
             */
            [[nodiscard]] std::shared_ptr<ziapi::config::Node> getValue(
                std::string const &root,
                std::string const &module,
                std::string const &param);

            /*
             * @brief Retrieves a vector to the path for every config file
             * @param[in] Path to the file containing every paths
             * @throw std::runtime_error if the config file is not found
             * @return A vector of string
             */
            [[nodiscard]] static std::vector<LibInfo> getConfigsPaths(std::string const &filename)
            {
                std::vector<LibInfo> libsInfos;
                std::ifstream file(filename);
                std::string line;

                if (!file.is_open())
                    throw std::runtime_error("[ConfigParser] Root to config file not found");
                while (std::getline(file, line)) {
                    std::size_t nameEndOffset = line.find_first_of("::");
                    if (nameEndOffset == std::string::npos)
                        throw std::runtime_error("[ConfigParser] Error while parsing config file");
                    std::size_t configPathOffset = line.find_first_of("::", nameEndOffset + 2);
                    if (configPathOffset == std::string::npos)
                        throw std::runtime_error("[ConfigParser] Error while parsing config file");
                    libsInfos.emplace_back(LibInfo{
                        line.substr(0, nameEndOffset),
                        line.substr(nameEndOffset + 2, configPathOffset - nameEndOffset - 2),
                        line.substr(configPathOffset + 2)
                    });
                }
                return (libsInfos);
            }

      private:
        inline void _checkConfig();
        void _putConfigInMap();

        YAML::Node _config;
        ziapi::config::Dict _configMap;
    };
} // namespace parser

#endif // CONFIG_PARSER_HPP
