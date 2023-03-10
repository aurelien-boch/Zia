#ifndef ENVMANAGER_HPP
#define ENVMANAGER_HPP

#include <vector>
#include <unordered_map>
#include <cstdio>

#include "VariableAlreadyDefinedException.hpp"
#include "VariableNotFoundException.hpp"

#if _WIN32
    #include <Windows.h>
#else
    #include <stdlib.h>
    #include <unistd.h>
#endif

#ifndef DllExport
    #ifdef _WIN32
        #define DllExport __declspec(dllexport)
    #else
        #define DllExport
    #endif
#endif

namespace env
{
    /**
     * @brief A small class that manages environment variables.
     */
    class DllExport Manager
    {
        public:
            /**
             * @brief Constructs an Manager from a C array of strings
             * @param[in] env The C string array
             */
            explicit Manager(char const **env);

            /**
             * @brief Constructs an Manager from a vector of C strings
             * @param[in] env A reference to the vector containing the C strings
             */
            explicit Manager(std::vector<char *> const &env);

            /**
             * @brief Constructs an empty Manager
             */
            explicit Manager() noexcept;

            /**
             * @brief Adds a new environment variable with name \bkey and value \bvalue
             * @param[in] key The name of the environment variable
             * @param[in] value The value of the environment variable
             * @throw VariableAlreadyDefinedException if the variable is already defined
             */
            void pushEnvVariable(std::string const &key, std::string const &value);

            /**
             * @brief Adds a new environment variable with name \bkey and value \bvalue
             * @param [in]key The name of the environment variable
             * @param [in]value The value of the environment variable
             * @throw VariableAlreadyDefinedException if the variable is already defined
             */
            void pushEnvVariable(char const *key, char const *value);

            /**
             * @brief Remove the environment variable with name \bkey
             * @param [in]key The name of the variable to remove
             * @throw VariableNotDefinedException if the variable is not defined
             */
            void removeEnvVariable(std::string const &key);

            /**
             * @brief Remove the environment variable with name \bkey
             * @param [in]key The name of the variable to remove
             * @throw VariableNotDefinedException if the variable is not defined
             */
            void removeEnvVariable(char const *key);

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] const std::string &getEnvVariable(std::string const &key) const;

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] std::string &getEnvVariable(std::string const &key);

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] const std::string &getEnvVariable(char const *key) const;

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] std::string &getEnvVariable(char const *key);

            /**
             * @brief Writes the environment to the system
             * @note On Windows, only the environment of the children will be affected
             */
            void writeToEnvironment() const;

            /**
             * @brief Loads the environment variables into the Manager
             */
            void loadFromEnvironment();

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] inline std::string &operator[](std::string const &str);

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] inline std::string &operator[](char const *str);

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] inline const std::string &operator[](std::string const &str) const;

            /**
             * @brief Returns the environment variable with name \bkey
             * @param [in]key The name of the variable to return
             * @return The environment variable as a reference to an std::string
             * @throw VariableNotDefinedException if the variable is not defined
             */
            [[nodiscard]] inline const std::string &operator[](char const *str) const;

            /**
             * @brief Converts the environment to a C style string array.
             * @return The C style array of C strings.
             */
            [[nodiscard]] operator char **() const noexcept;

            /**
             * @brief Converts the environment to C style strings.
             * @return The C style string, each string are null terminated an the final string contains two null bytes.
             */
            [[nodiscard]] operator std::string() const noexcept;

        private:
            std::unordered_map<std::string, std::string> _internalEnv;
    };
}

#endif //ENVMANAGER_HPP
