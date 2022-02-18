#include <cstring>

#include "HttpParser.hpp"
#include "Exception.hpp"

ziapi::http::Request HttpParser::HttpParser::parse(const std::string &requestString)
{
    std::size_t pos{};

    return {
        .method{parseRequestMethod(pos, requestString)},
        .target{parseRequestTarget(pos, requestString)},
        .version{parseRequestVersion(pos, requestString)},
        .fields{parseRequestHeaders(pos, requestString)},
        .body{parseRequestBody(pos, requestString)},
    };
}

inline std::string HttpParser::HttpParser::parseRequestMethod(std::size_t &pos, const std::string &requestString) const
{
    auto result{std::find_if(s_methods.begin() + pos, s_methods.end(),
                 [&](const std::string_view &method)
                 {return requestString.starts_with(method);})};

    if (result == s_methods.end()) {
        std::string wrongMethod{};
        requestString.copy(wrongMethod.data(), requestString.find_first_of(' '));
        throw InvalidMethodException{wrongMethod + " is not a valid HTTP method"};
    }
    pos += (*result).size() + 1;

    return std::string{*result};
}

inline std::string HttpParser::HttpParser::parseRequestTarget(std::size_t &pos, const std::string &requestString)
{
    std::string target{};
    requestString.copy(target.data(), requestString.find_first_of(' '), pos);

    if (target[0] != '/') {
        throw InvalidTargetException{"Target must start with a '/'"};
    }
    pos += target.size() + 1;

    return target;
}

inline ziapi::http::Version HttpParser::HttpParser::parseRequestVersion(std::size_t &pos,
                                                                 const std::string &requestString) const
{
    std::string version{};
    requestString.copy(version.data(), requestString.find_first_of('\r'), pos);

    if (s_versions.contains(version)) {
        pos += version.size() + 2;
        return s_versions.at(version);
    }

    throw InvalidVersionException{"Version is not supported or not valid"};
}

inline std::map<std::string, std::string> HttpParser::HttpParser::parseRequestHeaders(std::size_t &pos,
                                                                               const std::string &requestString) const
{
    std::map<std::string, std::string> headers{};
    std::string headerName{};
    std::string value{};

    while (requestString.find_first_of("\r\n\r\n", pos) != pos) {
        requestString.copy(headerName.data(), requestString.find_first_of(':', pos), pos);
        pos += headerName.size() + 2;

        for (auto &headersName: s_headersNames) {
            if (headersName == headerName) {
                requestString.copy(value.data(), requestString.find_first_of('\r', pos), pos);
                pos += value.size() + 2;
                break;
            }
            value.clear();
        }

        if (!headers.contains(headerName)) {
            throw InvalidHeaderException{"Header `" + std::string(headerName) + "` is not supported or not valid"};
        }
        value.clear();
        headerName.clear();
    }
    pos += 4;

    return headers;
}

inline std::string HttpParser::HttpParser::parseRequestBody(size_t &pos, const std::string &requestString)
{
    std::string body{};
    requestString.copy(body.data(), requestString.size() - pos, pos);

    return body;
}
