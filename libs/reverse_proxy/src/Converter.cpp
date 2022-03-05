#include <stdexcept>
#include "Converter.hpp"

namespace modules
{
    const std::unordered_map<std::string_view, ziapi::http::Version> Converter::_reversedVersions {
        {"HTTP/1.0", ziapi::http::Version::kV1},
        {"HTTP/1.1", ziapi::http::Version::kV1_1},
        {"HTTP/2.0", ziapi::http::Version::kV2},
        {"HTTP/3.0", ziapi::http::Version::kV3},
    };

    const std::unordered_map<ziapi::http::Version, std::string_view> Converter::_versions{
        {ziapi::http::Version::kV1, "HTTP/1.0"},
        {ziapi::http::Version::kV1_1, "HTTP/1.1"},
        {ziapi::http::Version::kV2, "HTTP/2.0"},
        {ziapi::http::Version::kV3, "HTTP/3.0"},
    };

    std::string Converter::toRawRequest(const ziapi::http::Request &req)
    {
        std::string res{};

        res
            .append(req.method)
            .append(" ")
            .append(req.target)
            .append(" ")
            .append(_versions.at(req.version))
            .append(" ")
            .append(" \r\n");
        for (auto const &[key, val] : req.headers)
            res.append(key).append(": ").append(val).append("\r\n");
        res.append("\r\n").append(req.body);
        return res;
    }

    ziapi::http::Response Converter::toResponse(std::string const &response)
    {
        ziapi::http::Response res;
        std::size_t body = response.find("\r\n\r\n");

        try {
            if (body == std::string::npos)
                throw std::runtime_error{""};
            _parseFirstLine(res, response);
            _parseHeaders(res, response);
            res.body = response.substr(body + 4);
        } catch (std::exception const &) {
            res.status_code = ziapi::http::Code::kServiceUnavailable;
        }
        return res;
    }

    void Converter::_parseFirstLine(ziapi::http::Response &response, std::string const &stringResponse)
    {
        std::size_t end{stringResponse.find("\r\n")};
        std::size_t endMethod{stringResponse.find(' ')};
        std::size_t endCode{stringResponse.find(' ', endMethod + 1)};

        if (end == std::string::npos || endMethod > end || endCode > end)
            throw std::runtime_error{""};
        response.version = _reversedVersions.at(stringResponse.substr(0, endMethod));
        response.status_code = static_cast<ziapi::http::Code>(std::stoi(stringResponse.substr(endMethod, endMethod - endCode)));
        response.reason = stringResponse.substr(endCode, end - endCode);
    }

    void Converter::_parseHeaders(ziapi::http::Response &response, std::string const &stringResponse)
    {
        std::size_t beginning = stringResponse.find("\r\n");
        std::size_t end = stringResponse.find("\r\n\r\n");
        std::size_t next;
        std::size_t separator;

        while (beginning < end)
        {
            next = stringResponse.find("\r\n", beginning + 1);
            separator = stringResponse.find(':', beginning);

            response.headers.try_emplace(
                stringResponse.substr(beginning + 2, separator - (beginning + 2)),
                stringResponse.substr((stringResponse[separator] == ' ') ? separator + 3 : separator + 2, next - (separator + 2))
            );
            beginning = next;
        }
    }
}
