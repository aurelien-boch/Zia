#include <sstream>

#include "HttpFormatter.hpp"

std::string formatter::HttpFormatter::format(const ziapi::http::Response &response)
{
    std::ostringstream responseStream{};

    responseStream << s_versions.at(response.version) << ' ' <<
                        static_cast<int>(response.status_code) << ' ' <<
                        response.reason << "\r\n";
    for (const auto &header : response.headers)
        responseStream << header.first << ": " << header.second << "\r\n";
    responseStream << "\r\n" << response.body;

    return responseStream.str();
}
