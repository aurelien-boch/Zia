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

    static constexpr std::array<std::string_view, 7> s_methods = {
            ziapi::http::method::kGet,
            ziapi::http::method::kPost,
            ziapi::http::method::kPut,
            ziapi::http::method::kDelete,
            ziapi::http::method::kPatch,
            ziapi::http::method::kOptions,
            ziapi::http::method::kHead,
    };

    const std::map<std::string_view, ziapi::http::Version> s_versions = {
            {"HTTP/1.0", ziapi::http::Version::kV1},
            {"HTTP/1.1", ziapi::http::Version::kV1_1},
            {"HTTP/2.0", ziapi::http::Version::kV2},
            {"HTTP/3.0", ziapi::http::Version::kV3},
    };

    [[nodiscard]] inline std::string parseRequestMethod(std::size_t &pos, const std::string &requestString) const;

    [[nodiscard]] static inline std::string parseRequestTarget(std::size_t &pos, const std::string &requestString);

    [[nodiscard]] inline ziapi::http::Version parseRequestVersion(std::size_t &pos, const std::string &requestString)
    const;
};

}

#endif /* ZIA_HTTPPARSER_HPP */
