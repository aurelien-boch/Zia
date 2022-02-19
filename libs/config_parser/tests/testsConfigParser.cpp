#include <catch2/catch_test_macros.hpp>

#include "ConfigParser.hpp"

TEST_CASE("static parsing of config paths", "[config_parser]")
{
    auto paths = parser::ConfigParser::getConfigsPaths("../libs/config_parser/tests/test_file.txt");

    REQUIRE(paths.size() == 1);
}

TEST_CASE("static parsing error : file not found", "[config_parser]")
{
    REQUIRE_THROWS_AS(parser::ConfigParser::getConfigsPaths("../libs/config_parser/tests/test_file_not_found.txt"), std::runtime_error);
}

TEST_CASE("creation of a config parser and check of a valid value in the config", "[config_parser]")
{
    parser::ConfigParser parser("../libs/config_parser/tests/config.yml");

    REQUIRE(parser.getValue("modules", "phpCGI", "foo")->AsString() == "bar");
}

TEST_CASE("creation of a config parser and check of an invalid value in the config", "[config_parser]")
{
    parser::ConfigParser parser("../libs/config_parser/tests/config.yml");

    REQUIRE_THROWS_AS(parser.getValue("modules", "phpCGI", "foot")->AsString(), std::range_error);
}

TEST_CASE("creation of a config parser and check of a valid value in the config with an invalid root", "[config_parser]")
{
    parser::ConfigParser parser("../libs/config_parser/tests/config.yml");

    REQUIRE_THROWS_AS(parser.getValue("module", "phpCGI", "foo")->AsString(), std::range_error);
}

TEST_CASE("creation of a config parser and check of a valid value in the config with an invalid module", "[config_parser]")
{
    parser::ConfigParser parser("../libs/config_parser/tests/config.yml");

    REQUIRE_THROWS_AS(parser.getValue("modules", "phpCGI_", "foo")->AsString(), std::range_error);
}

TEST_CASE("creation of a config parser with an empty config file", "[config_parser]")
{
    REQUIRE_THROWS_AS(parser::ConfigParser("../libs/config_parser/tests/empty_config.yml"), std::runtime_error);
}

TEST_CASE("creation of a config parser with a not config file", "[config_parser]")
{
    REQUIRE_THROWS_AS(parser::ConfigParser("../libs/config_parser/tests/config_not_found.yml"), std::runtime_error);
}

TEST_CASE("creation of a config parser and access via the operator", "[config_parser]")
{
    parser::ConfigParser parser("../libs/config_parser/tests/config.yml");

    REQUIRE(parser["phpCGI"]["foo"]->AsString() == "bar");
}

TEST_CASE("creation of a config parser and access via the operator with an invalid module", "[config_parser]")
{
    parser::ConfigParser parser("../libs/config_parser/tests/config.yml");

    REQUIRE_THROWS_AS(parser["phpCGI_"]["foo"], std::range_error);
}
