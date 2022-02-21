#include <catch2/catch_test_macros.hpp>
#include <string_view>

#include "HttpParser.hpp"
#include "ziapi/Http.hpp"

TEST_CASE("Valid HTTP request", "[HttpParser]")
{
    const std::string requestString{"GET / HTTP/1.1\r\n"
                                   "User-Agent: PostmanRuntime/7.29.0\r\n"
                                   "Accept: */*\r\n"
                                   "Cache-Control: no-cache\r\n"
                                   "Postman-Token: ffa0b8c8-2d43-4805-9af2-7cfac22dc7f3\r\n"
                                   "Host: localhost:8080\r\n"
                                   "Accept-Encoding: gzip, deflate, br\r\n"
                                   "Connection: keep-alive\r\n"
                                   "Cookie: token=github|36404435\r\n\r\n"
                                   "Hello World!"
    };
    ziapi::http::Request requestStruct{
        .version{ziapi::http::Version::kV1_1},
        .method{ziapi::http::method::kGet},
        .target{"/"},
        .fields{
            {"User-Agent", "PostmanRuntime/7.29.0"},
            {"Accept", "*/*"},
            {"Cache-Control", "no-cache"},
            {"Postman-Token", "ffa0b8c8-2d43-4805-9af2-7cfac22dc7f3"},
            {"Host", "localhost:8080"},
            {"Accept-Encoding", "gzip, deflate, br"},
            {"Connection", "keep-alive"},
            {"Cookie", "token=github|36404435"},
        },
        .body{"Hello World!"},
    };

    parser::HttpParser parser{};
    auto request{parser.parse(requestString)};

    REQUIRE(request.method == requestStruct.method);
    REQUIRE(request.target == requestStruct.target);
    REQUIRE(request.version == requestStruct.version);
    REQUIRE(request.fields == requestStruct.fields);
    REQUIRE(request.body == requestStruct.body);
}