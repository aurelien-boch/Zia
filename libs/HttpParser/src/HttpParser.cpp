#include <cstring>

#include "HttpParser.hpp"
#include "Exception.hpp"

ziapi::http::Request HttpParser::HttpParser::parse(const std::string &requestString)
{
    std::size_t pos{};
    std::size_t contentLength{};
    return {
        .method{parseRequestMethod(pos, requestString)},
        .target{parseRequestTarget(pos, requestString)},
        .version{parseRequestVersion(pos, requestString)},
        .fields{parseRequestHeaders(pos, requestString, contentLength)},
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

ziapi::http::Version HttpParser::HttpParser::parseRequestVersion(std::size_t &pos,
                                                                 const std::string &requestString) const
{
    char *version = new char;
    requestString.copy(version, requestString.find_first_of('\r'), pos);

    if (s_versions.contains(version)) {
        pos += strlen(version) + 2;
        return s_versions.at(version);
    }
    throw InvalidVersionException{"Version is not supported or not valid"};
}

std::map<std::string, std::string> HttpParser::HttpParser::parseRequestHeaders(std::size_t &pos,
                                                                               const std::string &requestString,
                                                                               std::size_t &contentLength) const
{
    std::map<std::string, std::string> headers;

    while (requestString.find_first_of("\r\n\r\n", pos) != pos) {
        char *headerName = new char;
        requestString.copy(headerName, requestString.find_first_of(':', pos), pos);
        pos += strlen(headerName) + 2;
        for (auto &headersName: s_headersNames) {
            if (headersName == headerName) {
                char *value = new char;
                requestString.copy(value, requestString.find_first_of('\r', pos), pos);
                pos += strlen(value) + 2;
                if (headersName == "Content-Length") {
                    contentLength = std::stoi(value);
                }
                break;
            }
        }
        if (!headers.contains(headerName)) {
            throw InvalidHeaderException{"Header `" + std::string(headerName) + "` is not supported or not valid"};
        }
    }

    pos += 4;

    return headers;
}

std::string
HttpParser::HttpParser::parseRequestBody(size_t &pos, const std::string &requestString, size_t &contentLength) const
{
    if (contentLength == 0) {
        return {};
    }

    std::string body{requestString.substr(pos, contentLength)};
}
