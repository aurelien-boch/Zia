#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <string>
#include <ziapi/Http.hpp>

namespace modules
{
    class Converter
    {
        public:
            static std::string toRawRequest(ziapi::http::Request const &req);

            static ziapi::http::Response toResponse(std::string const &response);

        private:
            static const std::unordered_map<ziapi::http::Version, std::string_view> _versions;

            static const std::unordered_map<std::string_view, ziapi::http::Version> _reversedVersions;

            static void _parseFirstLine(ziapi::http::Response &response, std::string const &stringResponse);

            static void _parseHeaders(ziapi::http::Response &response, std::string const &stringResponse);
    };
}

#endif //CONVERTER_HPP
