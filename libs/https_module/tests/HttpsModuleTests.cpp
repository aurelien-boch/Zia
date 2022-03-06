#include <catch2/catch_test_macros.hpp>

#include <ConfigParser.hpp>

#include <HttpsModule.hpp>

TEST_CASE("Init: Valid config with certificate", "[HttpsModule]")
{
    parser::ConfigParser parser("../libs/https_module/tests/validConfig.yml");
    modules::HttpsModule https;

    REQUIRE_NOTHROW(https.Init(parser.getConfigMap()));
}

TEST_CASE("Init: Valid config without certificate", "[HttpsModule]")
{
    parser::ConfigParser parser("../libs/https_module/tests/validConfigWithoutCertificate.yml");
    modules::HttpsModule https;

    REQUIRE_NOTHROW(https.Init(parser.getConfigMap()));
}

TEST_CASE("Init: invalid port", "[HttpsModule]")
{
    parser::ConfigParser parser("../libs/https_module/tests/invalidPortConfig.yml");
    modules::HttpsModule https;

    REQUIRE_NOTHROW(https.Init(parser.getConfigMap()));
}

TEST_CASE("Init: no module", "[HttpsModule]")
{
    parser::ConfigParser parser("../libs/https_module/tests/noModuleConfig.yml");
    modules::HttpsModule https;

    REQUIRE_THROWS(https.Init(parser.getConfigMap()));
}