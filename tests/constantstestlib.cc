#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "constants/constantslib.hpp"

TEST_CASE( "Constants", "[main]" )
{
    REQUIRE (constants::EMPTY == 0xFF);
    REQUIRE (constants::EIGHT_PUZZLE_SIZE == 3);
    REQUIRE (constants::EIGHT_PUZZLE_NUM == 9);
    REQUIRE (constants::RIGHT == 0);
    REQUIRE (constants::UP == 1);
    REQUIRE (constants::LEFT == 2);
    REQUIRE (constants::DOWN == 3);
}
