#include <catch2/catch_test_macros.hpp>

#include <ConfigParser.hpp>

#include <HttpModule.hpp>

TEST_CASE("Config basic", "[HttpModule]")
{
    parser::ConfigParser parser("../libs/http_module/tests/validConfig.yml");
    modules::HttpModule http;

    REQUIRE_NOTHROW(http.Init(parser.getConfigMap()));
}

TEST_CASE("Config invalid port", "[HttpModule]")
{
    parser::ConfigParser parser("../libs/http_module/tests/invalidPortConfig.yml");
    modules::HttpModule http;

    REQUIRE_NOTHROW(http.Init(parser.getConfigMap()));
}

TEST_CASE("Config no module", "[HttpModule]")
{
    parser::ConfigParser parser("../libs/http_module/tests/noModuleConfig.yml");
    modules::HttpModule http;

    REQUIRE_THROWS(http.Init(parser.getConfigMap()));
}