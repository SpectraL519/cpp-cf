#include "../doctest.h"

#include <cf/chainable_functor.hpp>


TEST_CASE("ChainableFunctor should use correct types") {
    // TODO:
    // * Parametrized test
    // * Check types for all cf type specifiers

    /*
    REQUIRE(std::is_same_v<typename adder_type::type, int>);
    REQUIRE(std::is_same_v<typename adder_type::return_type, int>);
    REQUIRE(std::is_same_v<typename adder_type::argument_type, const int>);
    REQUIRE(std::is_same_v<typename adder_type::function_type,
                           std::function<int(const int, const int)>>);
    */

    CHECK(true);
}
