#include "HttpParser.hpp"

#include "Exception.hpp"

ziapi::http::Request HttpParser::HttpParser::parse(const std::string &requestString)
{
    ziapi::http::Request request{};
    std::size_t pos{};

    request.method = parseRequestMethod(pos, requestString);

    return request;
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