#include <cstring>

#include "HttpParser.hpp"
#include "Exception.hpp"

ziapi::http::Request parser::HttpParser::parse(const std::string &requestString)
{
    std::size_t pos{};
    auto method{_parseRequestMethod(pos, requestString)};
    auto target{_parseRequestTarget(pos, requestString)};
    auto version{_parseRequestVersion(pos, requestString)};
    auto fields(_parseRequestHeaders(pos, requestString));
    auto body{_parseRequestBody(pos, requestString)};

    return {
        .version{version},
        .target{target},
        .method{method},
        .headers{fields},
        .body{body},
    };
}

inline std::string parser::HttpParser::_parseRequestMethod(std::size_t &pos, const std::string &requestString) const
{
    auto result{std::find_if(s_methods.begin() + pos, s_methods.end(),
                 [&](const std::string_view &method)
                 {return requestString.starts_with(method);})};

    if (result == s_methods.end()) {
        std::string wrongMethod{};
        requestString.copy(wrongMethod.data(), requestString.find_first_of(' ', pos), pos);
        throw InvalidMethodException{wrongMethod + " is not a valid HTTP method"};
    }
    pos += (*result).size() + 1;
    return std::string{*result};
}

inline std::string parser::HttpParser::_parseRequestTarget(std::size_t &pos, const std::string &requestString)
{
    std::string target{requestString.substr(pos, requestString.find_first_of(' ', pos) - pos)};

    if (target[0] != '/')
        throw InvalidTargetException{"Target must start with a '/'"};
    pos += target.size() + 1;
    return target;
}

inline ziapi::http::Version parser::HttpParser::_parseRequestVersion(std::size_t &pos,
                                                                     const std::string &requestString) const
{
    std::string version{requestString.substr(pos, requestString.find_first_of('\r', pos) - pos)};

    if (!s_versions.contains(version))
        throw InvalidVersionException{"Version is not supported or not valid"};
    pos += version.size() + 2;
    return s_versions.at(version);
}

inline std::map<std::string, std::string> parser::HttpParser::_parseRequestHeaders(std::size_t &pos,
                                                                                   const std::string &requestString) const
{
    std::map<std::string, std::string> headers{};
    std::string headerName{};
    std::string value{};

    while (requestString.find_first_of("\r\n", pos) != pos) {
        headerName = requestString.substr(pos, requestString.find_first_of(':', pos) - pos);
        pos += headerName.size() + 2;
        value = requestString.substr(pos, requestString.find_first_of('\r', pos) - pos);
        pos += value.size() + 2;
        headers.emplace(headerName, value);

        if (!headers.contains(headerName))
            throw InvalidHeaderException{"Header `" + std::string(headerName) + "` is not supported or not valid"};
        value.clear();
        headerName.clear();
    }
    pos += 2;
    return headers;
}

inline std::string parser::HttpParser::_parseRequestBody(size_t &pos, const std::string &requestString)
{
    std::string body{requestString.substr(pos, requestString.size() - pos)};

    return body;
}
