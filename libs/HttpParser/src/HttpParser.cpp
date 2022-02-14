#include "HttpParser.hpp"

#include "Exception.hpp"

ziapi::http::Request HttpParser::HttpParser::parse(const std::string &requestString)
{
    std::size_t pos{};
    return {
        .method{parseRequestMethod(pos, requestString)},
        .target{parseRequestTarget(pos, requestString)},
    };
}

inline std::string HttpParser::HttpParser::parseRequestMethod(std::size_t &pos, const std::string &requestString) const
{
    auto result{std::find_if(s_methods.begin() + pos, s_methods.end(),
                 [&](const std::string_view &method)
                 {return requestString.starts_with(method);})};

    if (result == s_methods.end()) {
        char *wrongMethod = new char;
        requestString.copy(wrongMethod, requestString.find_first_of(' '));
        throw InvalidMethodException{std::string{wrongMethod} + " is not a valid HTTP method"};
    }

    pos += (*result).size() + 1;
    return std::string{*result};
}

inline std::string HttpParser::HttpParser::parseRequestTarget(std::size_t &pos, const std::string &requestString)
{
    char *target = new char;
    requestString.copy(target, requestString.find_first_of(' '), pos);

    if (target[0] != '/') {
        throw InvalidTargetException{"Target must start with a '/'"};
    }

    std::string targetString{target};
    pos += targetString.size() + 1;
    return targetString;
}
