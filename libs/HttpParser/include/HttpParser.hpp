#ifndef ZIA_HTTPPARSER_HPP
#define ZIA_HTTPPARSER_HPP

#include "ziapi/Http.hpp"

namespace HttpParser {

class HttpParser {

public:

    HttpParser() = default;
    ~HttpParser() = default;

    HttpParser(HttpParser &other) = delete;
    HttpParser(HttpParser &&other) noexcept = delete;
    HttpParser operator=(HttpParser &other) = delete;

    ziapi::http::Request parse(const std::string &requestString);

private:

    std::array<std::string_view, 7> s_methods = {
            ziapi::http::method::kGet,
            ziapi::http::method::kPost,
            ziapi::http::method::kPut,
            ziapi::http::method::kDelete,
            ziapi::http::method::kPatch,
            ziapi::http::method::kOptions,
            ziapi::http::method::kHead,
    };

    std::array<ziapi::http::Version, 4> s_versions = {
            ziapi::http::Version::kV1,
            ziapi::http::Version::kV1_1,
            ziapi::http::Version::kV2,
            ziapi::http::Version::kV3,
    };

    [[nodiscard]] inline std::string parseRequestMethod(std::size_t &pos, const std::string &requestString) const;
};

}

#endif /* ZIA_HTTPPARSER_HPP */
