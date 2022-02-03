#include <catch2/catch_test_macros.hpp>

#include <Loader.hpp>

TEST_CASE("All ok", "[loader]")
{
    loader::Loader loader(MOCK_LIB_PATH);
    auto symbol = loader.getSymbol<int ()>("symbol");

    REQUIRE(symbol() == 12345);
}

TEST_CASE("Unknown symbol", "[loader]")
{
    loader::Loader loader(MOCK_LIB_PATH);
    try {
        auto symbol = loader.getSymbol<int ()>("unknown symbol");
        FAIL();
    } catch (std::runtime_error &e) {
        SUCCEED();
    }
}

TEST_CASE("Unknown library", "[loader]")
{
    try {
        loader::Loader loader("libliblib" MOCK_LIB_PATH);
        FAIL();
    } catch (std::runtime_error &e) {
        SUCCEED();
    }
}
