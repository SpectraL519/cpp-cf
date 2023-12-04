#include "../doctest.h"

#include <cf/chainable_functor.hpp>


TEST_CASE("ChainableFunctor shoul be initialized correctly by move") {
    const cf::ChainableFunctor<int, cf::value, cf::const_value> adder{
        [](const int a, const int b) {
            return a + b;
        }
    };

    // TODO: check if the class is correctly

    CHECK(true);
}
