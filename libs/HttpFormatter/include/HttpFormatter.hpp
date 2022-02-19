#ifndef ZIA_HTTPFORMATTER_HPP
#define ZIA_HTTPFORMATTER_HPP

#include <string>

#include "ziapi/Http.hpp"

namespace Http::Formatter {

/**
 * @brief Formats Http responses into a string
 */
class HttpFormatter {

public:

    HttpFormatter() = default;
    ~HttpFormatter() = default;

    HttpFormatter(const HttpFormatter &other) = delete;
    HttpFormatter(const HttpFormatter &&other) = delete;
    HttpFormatter &operator=(const HttpFormatter &other) = delete;

    /**
     * @brief Formats an Http response into a string to send back to a client
     *
     * @param response The response to format
     * @return the formatted response
     */
    std::string format(const ziapi::http::Response &response);

private:

    /// Version associated between the string in the response and the enum
    const std::unordered_map<ziapi::http::Version, std::string_view> s_versions{
        {ziapi::http::Version::kV1, "HTTP/1.0"},
        {ziapi::http::Version::kV1_1, "HTTP/1.1"},
        {ziapi::http::Version::kV2, "HTTP/2.0"},
        {ziapi::http::Version::kV3, "HTTP/3.0"},
    };
};

}

#endif /* ZIA_HTTPFORMATTER_HPP */
