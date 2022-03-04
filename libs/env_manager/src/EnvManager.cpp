#include <stdexcept>
#include <cstring>

#include "EnvManager.hpp"

namespace env
{
    Manager::Manager( char const **env) :
        _internalEnv {}
    {
        for (std::size_t i = 0; env[i] != nullptr; ++i) {
            const std::string converted{env[i]};
            const auto pos{static_cast<std::int64_t>(converted.find('='))};
            std::string key{converted.cbegin(), converted.cbegin() + pos};
            std::string value{converted.cbegin() + pos + 1, converted.cend()};

            _internalEnv.insert({std::move(key), std::move(value)});
        }
    }

    Manager::Manager(const std::vector<char *> &env) :
        _internalEnv{}
    {
        for (auto const &e: env) {
            const std::string converted{e};
            const auto pos{static_cast<int64_t>(converted.find('='))};
            std::string key{converted.cbegin(), converted.cbegin() + pos};
            std::string value{converted.cbegin() + pos + 1, converted.cend()};

            _internalEnv.insert({std::move(key), std::move(value)});
        }
    }

    Manager::Manager() noexcept :
        _internalEnv{}
    {}

    void Manager::pushEnvVariable(std::string const &key, std::string const &value)
    {
        auto [_, success] = _internalEnv.insert({key, value});

        if (!success)
            throw VariableAlreadyDefinedException{key};
    }

    void Manager::pushEnvVariable(char const *key, char const *value)
    {
        auto [_, success] = _internalEnv.insert({key, value});

        if (!success)
            throw VariableAlreadyDefinedException{key};
    }

    void Manager::removeEnvVariable(const std::string &key)
    {
        std::size_t success = _internalEnv.erase(key);

        if (success == 0)
            throw VariableNotDefinedException{key};
    }

    void Manager::removeEnvVariable(const char *key)
    {
        std::size_t success{_internalEnv.erase(key)};

        if (success == 0)
            throw VariableNotDefinedException{key};
    }

    const std::string &Manager::getEnvVariable(const std::string &key) const
    {
        try {
            return _internalEnv.at(key);
        } catch (std::out_of_range &e) {
            throw VariableNotDefinedException{key};
        }
    }

    std::string &Manager::getEnvVariable(const std::string &key)
    {
        try {
            return _internalEnv.at(key);
        } catch (std::out_of_range &e) {
            throw VariableNotDefinedException{key};
        }
    }

    const std::string &Manager::getEnvVariable(const char *key) const
    {
        try {
            return _internalEnv.at(key);
        } catch (std::out_of_range &e) {
            throw VariableNotDefinedException{key};
        }
    }

    std::string &Manager::getEnvVariable(const char *key)
    {
        try {
            return _internalEnv.at(key);
        } catch (std::out_of_range &e) {
            throw VariableNotDefinedException{key};
        }
    }

    void Manager::writeToEnvironment() const
    {
        for (auto const &[key, val] : _internalEnv)
#if _WIN32
            if (SetEnvironmentVariable(key.c_str(), val.c_str()) == 0)
                throw std::runtime_error{"An error occurred while setting environment"};
#else
        {
            std::string res{key};

            res += '=';
            res += val;
            if (putenv(res.data()) == 0)
                throw std::runtime_error{strerror(errno)};
        }
#endif
    }

    void Manager::loadFromEnvironment()
    {
        std::vector<std::string> env{};
#if _WIN32
        char const *data = GetEnvironmentStrings();
        std::size_t old_pos = 0;

        for (std::size_t i = 0; true; ++i)
            if (data[i] == '\0') {
                if (data[i + 1] == '\0')
                    break;
                else {
                    env.emplace_back(data + old_pos, data + i);
                    old_pos = i + 1;
                }
            }
        FreeEnvironmentStrings(const_cast<char *>(data));
        env.erase(env.begin(), env.begin() + 1);
#else
        for (std::size_t i = 0; environ[i] != nullptr; ++i)
            env.emplace_back(environ[i]);
#endif
        for (auto const &e : env) {
            auto delimiter = static_cast<std::int64_t>(e.find('='));

            try {
                this->pushEnvVariable(
                    {e.c_str(), e.c_str() + delimiter},
                    {e.c_str() + delimiter + 1, e.c_str() + e.size()}
                );
            } catch (VariableAlreadyDefinedException &e) {}
        }
    }

    std::string &Manager::operator[](const std::string &str)
    {
        return this->getEnvVariable(str);
    }

    std::string &Manager::operator[](const char *str)
    {
        return this->getEnvVariable(str);
    }

    const std::string &Manager::operator[](const std::string &str) const
    {
        return this->getEnvVariable(str);
    }

    const std::string &Manager::operator[](const char *str) const
    {
        return this->getEnvVariable(str);
    }

    Manager::operator char **() const noexcept
    {
        char **res = new char *[_internalEnv.size() + 1];
        std::size_t pos{};

        for (auto const &e: _internalEnv) {
            std::string tmp{e.first + "=" + e.second};
            res[pos] = new char[tmp.size() + 1];
            std::memcpy(res[pos++], tmp.c_str(), tmp.size() + 1);
        }
        res[_internalEnv.size()] = nullptr;
        return (res);
    }

    Manager::operator std::string() const noexcept
    {
        std::string res{};

        for (auto const &e: _internalEnv)
            res += {e.first + "=" + e.second + '\0'};
        res += '\0';
        return (res);
    }
}
