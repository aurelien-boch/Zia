#include <catch2/catch_test_macros.hpp>

#include <Loader.hpp>

TEST_CASE("All ok", "[loader]")
{
    loader::Loader loader(MOCK_LIB_PATH);
    auto symbol = loader.getSymbol<int ()>("symbol");

    REQUIRE(symbol() == 12345);
}

//path
