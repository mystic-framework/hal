import mystic.version;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test Case", "[test]") {
  SECTION("Test Section") {
    REQUIRE(Return42() == 42);
    REQUIRE(ReturnTrue() == true);
  }
}
