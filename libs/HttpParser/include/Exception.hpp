#ifndef ZIA_EXCEPTION_HPP
#define ZIA_EXCEPTION_HPP

#include <exception>
#include <string_view>
#include <ostream>

namespace HttpParser {

class Exception : public std::exception {

public:

    explicit Exception(const std::string &error) noexcept : s_error{error} {}

    [[nodiscard]] inline const char *what() const noexcept override
    {
        return s_error.c_str();
    }

    std::ostream &operator<<(std::ostream &os) const noexcept
    {
        os << s_error;
        return os;
    }

protected:

    const std::string &s_error;

};

class InvalidMethodException : public Exception {

public:

    explicit InvalidMethodException(const std::string &error) noexcept : Exception{"[Invalid Method]: " + error} {}

};

class InvalidTargetException : public Exception {

public:

    explicit InvalidTargetException(const std::string &error) noexcept : Exception{"[Invalid Route]: " + error} {}

};

}

#endif /* ZIA_EXCEPTION_HPP */
