#include <type_traits>
#include <vector>

#include "../doctest.h"

#include <cf/chainable_functor.hpp>


TEST_CASE("ChainableFunctor::result should return correct value through implicit conversion") {
    using adder_type = cf::ChainableFunctor<int, cf::value, cf::const_value>;

    const auto add = [](const int a, const int b) { return a + b; };
    adder_type adder{add};

    REQUIRE(std::is_same_v<typename adder_type::Type, int>);
    REQUIRE(std::is_same_v<typename adder_type::ReturnType, int>);
    REQUIRE(std::is_same_v<typename adder_type::ArgumentType, const int>);
    REQUIRE(std::is_same_v<typename adder_type::FunctionType,
                           std::function<int(const int, const int)>>);

    int add_func_result, chainable_adder_result;

    SUBCASE("single argument") {
        const auto default_value = int{};
        const int argument = 1;

        add_func_result = add(default_value, argument);
        chainable_adder_result = adder(argument);
    }

    SUBCASE("two arguments") {
        const auto arguments = std::make_pair(1, 2);

        add_func_result = add(arguments.first, arguments.second);
        chainable_adder_result = adder(arguments.first)(arguments.second);
    }

    SUBCASE("multiple arguments") {
        const std::vector<int> arguments{1, 2, 3};

        add_func_result = add(add(arguments.at(0), arguments.at(1)), arguments.at(2));
        chainable_adder_result = adder(arguments.at(0))(arguments.at(1))(arguments.at(2));
    }

    CAPTURE(add_func_result);
    CAPTURE(chainable_adder_result);

    REQUIRE_EQ(add_func_result, chainable_adder_result);
}
