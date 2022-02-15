#ifndef ZIA_EXCEPTION_HPP
#define ZIA_EXCEPTION_HPP

#include <exception>
#include <string_view>
#include <ostream>

namespace HttpParser {

/**
 * Base Exception for the HttpParser module
 * It is intended that exceptions used for this module are derived from this class
 */
class Exception : public std::exception {

public:

    explicit Exception(const std::string &error) noexcept : s_error{error} {}

    [[nodiscard]] inline const char *what() const noexcept override
    {
        return s_error.c_str();
    }

    std::ostream &operator<<(std::ostream &os) const noexcept
    {
        os << s_logsPrefix << s_error;
        return os;
    }

protected:

    std::string s_logsPrefix{"[Exception] "};
    const std::string &s_error;

};

/**
 * To be thrown on invalid method name
 */
class InvalidMethodException : public Exception {

public:

    explicit InvalidMethodException(const std::string &error) noexcept : Exception{error}
    {
        s_logsPrefix = "[Invalid Method] ";
    }

};

/**
 * To be thrown on invalid target name
 */
class InvalidTargetException : public Exception {

public:

    explicit InvalidTargetException(const std::string &error) noexcept : Exception{error}
    {
        s_logsPrefix = "[Invalid Target] ";
    }

};

/**
 * To be thrown on invalid version name
 */
class InvalidVersionException : public Exception {

public:

    explicit InvalidVersionException(const std::string &error) noexcept : Exception{error}
    {
        s_logsPrefix = "[Invalid Version] ";
    }

};

/**
 * To be thrown on invalid header name
 */
class InvalidHeaderException : public Exception {

public:

    explicit InvalidHeaderException(const std::string &error) noexcept : Exception{error}
    {
        s_logsPrefix = "[Invalid Header] ";
    }

};

}

#endif /* ZIA_EXCEPTION_HPP */
