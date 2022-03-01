#include <sstream>
#include <iostream>

#include "HttpFormatter.hpp"

std::string formatter::HttpFormatter::format(const ziapi::http::Response &response)
{
    std::ostringstream responseStream{};

    try {
        responseStream <<
            s_versions.at(response.version)
            << ' '
            << static_cast<int>(response.status_code)
            << ' '
            << response.reason
            << "\r\n";
    } catch (std::out_of_range const &) {
        std::cerr << "ERROR(modules/http): Formatter error: invalid version provided." << std::endl;
    }
    for (const auto &header : response.headers)
        responseStream << header.first << ": " << header.second << "\r\n";
    responseStream << "\r\n" << response.body;

    return responseStream.str();
}
