#ifndef ZIA_HTTPFORMATTER_HPP
#define ZIA_HTTPFORMATTER_HPP

#include <string>

#include "ziapi/Http.hpp"

namespace Http::Formatter {

class HttpFormatter {

public:

    HttpFormatter() = default;
    ~HttpFormatter() = default;

    std::string_view format(const ziapi::http::Response &response);

private:

    const std::unordered_map<ziapi::http::Version, std::string_view> s_versions{
        {ziapi::http::Version::kV1, "HTTP/1.0"},
        {ziapi::http::Version::kV1_1, "HTTP/1.1"},
        {ziapi::http::Version::kV2, "HTTP/2.0"},
        {ziapi::http::Version::kV3, "HTTP/3.0"},
    };
};

}

#endif /* ZIA_HTTPFORMATTER_HPP */
