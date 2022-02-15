#ifndef ZIA_HTTPPARSER_HPP
#define ZIA_HTTPPARSER_HPP

#include "ziapi/Http.hpp"

namespace HttpParser {

/**
 * @brief Parses HTTP requests on each `parse` call
 */
class HttpParser {

public:

    HttpParser() = default;
    ~HttpParser() = default;

    HttpParser(HttpParser &other) = delete;
    HttpParser(HttpParser &&other) noexcept = delete;
    HttpParser operator=(HttpParser &other) = delete;

    /**
     * @brief Parse an HTTP request
     *
     * @param requestString The string containing the HTTP request
     * @return The parsed HTTP request
     */
    ziapi::http::Request parse(const std::string &requestString);

private:

    /**
     * The HTTP request supported methods
     */
    static constexpr std::array<std::string_view, 7> s_methods{
            ziapi::http::method::kGet,
            ziapi::http::method::kPost,
            ziapi::http::method::kPut,
            ziapi::http::method::kDelete,
            ziapi::http::method::kPatch,
            ziapi::http::method::kOptions,
            ziapi::http::method::kHead,
    };

    /**
     * The HTTP request supported versions
     */
    const std::map<std::string_view, ziapi::http::Version> s_versions{
            {"HTTP/1.0", ziapi::http::Version::kV1},
            {"HTTP/1.1", ziapi::http::Version::kV1_1},
            {"HTTP/2.0", ziapi::http::Version::kV2},
            {"HTTP/3.0", ziapi::http::Version::kV3},
    };

    /**
     * The HTTP request supported headers
     */
    static constexpr std::array<std::string_view, 76> s_headersNames{
            ziapi::http::header::kAIM,
            ziapi::http::header::kAccept,
            ziapi::http::header::kAcceptCharset,
            ziapi::http::header::kAcceptDatetime,
            ziapi::http::header::kAcceptEncoding,
            ziapi::http::header::kAcceptLanguage,
            ziapi::http::header::kAccessControlRequestMethod,
            ziapi::http::header::kAccessControlRequestHeaders,
            ziapi::http::header::kAuthorization,
            ziapi::http::header::kCacheControl,
            ziapi::http::header::kConnection,
            ziapi::http::header::kContentLength,
            ziapi::http::header::kContentMD5,
            ziapi::http::header::kContentType,
            ziapi::http::header::kCookie,
            ziapi::http::header::kDate,
            ziapi::http::header::kExpect,
            ziapi::http::header::kForwarded,
            ziapi::http::header::kFrom,
            ziapi::http::header::kHost,
            ziapi::http::header::kHTTP2Settings,
            ziapi::http::header::kIfMatch,
            ziapi::http::header::kIfModifiedSince,
            ziapi::http::header::kIfNoneMatch,
            ziapi::http::header::kIfRange,
            ziapi::http::header::kIfUnmodifiedSince,
            ziapi::http::header::kMaxForwards,
            ziapi::http::header::kOrigin,
            ziapi::http::header::kPragma,
            ziapi::http::header::kPrefer,
            ziapi::http::header::kProxyAuthorization,
            ziapi::http::header::kRange,
            ziapi::http::header::kReferer,
            ziapi::http::header::kTE,
            ziapi::http::header::kUserAgent,
            ziapi::http::header::kUpgrade,
            ziapi::http::header::kVia,
            ziapi::http::header::kAcceptCH,
            ziapi::http::header::kAccessControlAllowOrigin,
            ziapi::http::header::kAccessControlAllowCredentials,
            ziapi::http::header::kAccessControlExposeHeaders,
            ziapi::http::header::kAccessControlMaxAge,
            ziapi::http::header::kAccessControlAllowMethods,
            ziapi::http::header::kAccessControlAllowHeaders,
            ziapi::http::header::kAcceptPatch,
            ziapi::http::header::kAcceptRanges,
            ziapi::http::header::kAge,
            ziapi::http::header::kAllow,
            ziapi::http::header::kAltSvc,
            ziapi::http::header::kContentDisposition,
            ziapi::http::header::kContentEncoding,
            ziapi::http::header::kContentLanguage,
            ziapi::http::header::kContentLocation,
            ziapi::http::header::kContentRange,
            ziapi::http::header::kDeltaBase,
            ziapi::http::header::kETag,
            ziapi::http::header::kExpires,
            ziapi::http::header::kIM,
            ziapi::http::header::kLastModified,
            ziapi::http::header::kLink,
            ziapi::http::header::kLocation,
            ziapi::http::header::kP3P,
            ziapi::http::header::kPreferenceApplied,
            ziapi::http::header::kProxyAuthenticate,
            ziapi::http::header::kPublicKeyPins,
            ziapi::http::header::kRetryAfter,
            ziapi::http::header::kServer,
            ziapi::http::header::kSetCookie,
            ziapi::http::header::kStrictTransportSecurity,
            ziapi::http::header::kTrailer,
            ziapi::http::header::kTransferEncoding,
            ziapi::http::header::kTk,
            ziapi::http::header::kVary,
            ziapi::http::header::kWarning,
            ziapi::http::header::kWWWAuthenticate,
            ziapi::http::header::kXFrameOptions,
    };

    /**
     * Parses the method of the HTTP request
     * @param pos the starting position of what's left to parse
     * @param requestString the string containing the request
     * @return the parsed method
     */
    [[nodiscard]] inline std::string parseRequestMethod(std::size_t &pos, const std::string &requestString) const;

    /**
     * Parses the target of the HTTP request
     * @param pos the starting position of what's left to parse
     * @param requestString the string containing the request
     * @return the parsed target
     */
    [[nodiscard]] static inline std::string parseRequestTarget(std::size_t &pos, const std::string &requestString);

    /**
     * Parses the version of the HTTP request
     * @param pos the starting position of what's left to parse
     * @param requestString the string containing the request
     * @return the parsed version
     */
    [[nodiscard]] inline ziapi::http::Version parseRequestVersion(std::size_t &pos, const std::string &requestString)
    const;

    /**
     * Parses the headers of the HTTP request
     * @param pos the starting position of what's left to parse
     * @param requestString the string containing the request
     * @return the parsed headers
     */
    [[nodiscard]] inline std::map<std::string, std::string> parseRequestHeaders(std::size_t &pos,
                                                                                const std::string &requestString,
                                                                                std::size_t &contentLength) const;

    /**
     * Parses the body of the HTTP request
     * @param pos the starting position of what's left to parse
     * @param requestString the string containing the request
     * @return the parsed body
     */
    [[nodiscard]] static inline std::string parseRequestBody(std::size_t &pos, const std::string &requestString,
                                                                std::size_t &contentLength);
};

}

#endif /* ZIA_HTTPPARSER_HPP */
