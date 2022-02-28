#include <catch2/catch_test_macros.hpp>

#include <asio.hpp>

#define private public
    #include <AsioHttpClient.hpp>
#undef private

TEST_CASE("_getContentLength basic", "[AsioHttpClient]")
{
    const std::string requestString{"GET toto HTTP/1.1\r\n"
        "User-Agent: PostmanRuntime/7.29.0\r\n"
        "Accept: */*\r\n"
        "Cache-Control: no-cache\r\n"
        "Postman-Token: ffa0b8c8-2d43-4805-9af2-7cfaxc22dc7f3\r\n"
        "Host: localhost:8080\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 12\r\n"
        "Cookie: token=github|36404435\r\n\r\n"
        "Hello World!"
    };
    asio::io_context context{};
    network::http::AsioHttpClient client(context);

    REQUIRE(client._getContentLength(requestString) == 12);
}

TEST_CASE("_getContentLength no content length", "[AsioHttpClient]")
{
    const std::string requestString{"GET toto HTTP/1.1\r\n"
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
    asio::io_context context{};
    network::http::AsioHttpClient client(context);

    REQUIRE_THROWS(client._getContentLength(requestString));
}

TEST_CASE("_cleanHeader basic", "[AsioHttpClient]")
{
    asio::io_context context{};
    network::http::AsioHttpClient client(context);
    std::string header = "this is the header\r\n\r\nthis is the body";
    std::string body;

    client._cleanHeader(header, body);
    REQUIRE(header == "this is the header\r\n\r\n");
    REQUIRE(body == "this is the body");
}

TEST_CASE("_cleanHeader empty body", "[AsioHttpClient]")
{
    asio::io_context context{};
    network::http::AsioHttpClient client(context);
    std::string header = "this is the header\r\n\r\n";
    std::string body;

    client._cleanHeader(header, body);
    REQUIRE(header == "this is the header\r\n\r\n");
    REQUIRE(body == "");
}

TEST_CASE("_cleanHeader empty header", "[AsioHttpClient]")
{
    asio::io_context context{};
    network::http::AsioHttpClient client(context);
    std::string header = "\r\n\r\nthis is the body";
    std::string body;

    client._cleanHeader(header, body);
    REQUIRE(header == "\r\n\r\n");
    REQUIRE(body == "this is the body");
}

TEST_CASE("_cleanHeader no end", "[AsioHttpClient]")
{
    asio::io_context context{};
    network::http::AsioHttpClient client(context);
    std::string header = "this is the header and this is the body";
    std::string body;

    REQUIRE_THROWS(client._cleanHeader(header, body));
}