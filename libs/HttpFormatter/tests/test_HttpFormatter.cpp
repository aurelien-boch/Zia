#include <catch2/catch_test_macros.hpp>
#include <string_view>

#include "HttpFormatter.hpp"
#include "ziapi/Http.hpp"

TEST_CASE("Valid Http Response", "[HttpFormatter]")
{
    static constexpr std::string_view responseString = "HTTP/1.1 200 OK\r\n"
                           "Content-Length: 12\r\n"
                           "Content-Type: text/html\r\n"
                           "\r\n"
                           "<html>\n"
                           "<body>\n"
                           "Hello World!\n"
                           "</body>\n"
                           "</html>";
    static const ziapi::http::Response responseStruct{
        .version{ziapi::http::Version::kV1_1},
        .status_code{ziapi::http::Code::kOK},
        .reason{ziapi::http::reason::kOK},
        .fields{
            {"Content-Length", "12"},
            {"Content-Type", "text/html"},
        },
        .body{"<html>\n"
              "<body>\n"
              "Hello World!\n"
              "</body>\n"
              "</html>"
        },
    };

    Http::Formatter::HttpFormatter formatter{};
    auto response = formatter.format(responseStruct);

    REQUIRE(responseString == response);
}

TEST_CASE("Null Initialized Version", "[HttpFormatter]")
{
    static const ziapi::http::Response responseStruct{
        .version{},
        .status_code{ziapi::http::Code::kOK},
        .reason{ziapi::http::reason::kOK},
        .fields{
            {"Content-Length", "12"},
            {"Content-Type", "text/html"},
        },
        .body{"<html>\n"
              "<body>\n"
              "Hello World!\n"
              "</body>\n"
              "</html>"
        },
    };
    Http::Formatter::HttpFormatter formatter{};

    try {
        auto response = formatter.format(responseStruct);
        FAIL();
    } catch (const std::out_of_range &e) {
        SUCCEED();
    }
}

TEST_CASE("Null Initialized Response", "[HttpFormatter]")
{
    ziapi::http::Response responseStruct{};
    Http::Formatter::HttpFormatter formatter{};

    try {
        auto response = formatter.format(responseStruct);
        FAIL();
    } catch (const std::out_of_range &e) {
        SUCCEED();
    }
}