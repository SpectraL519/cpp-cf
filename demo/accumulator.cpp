#include <cf/chainable_functor.hpp>

#include <iostream>
#include <type_traits>

template <typename T>
requires std::is_arithmetic_v<T>
auto make_accumulator() {
    return cf::chainable_functor<T>{
        [](T a, T b) {
            return a + b;
        },
        T{}  // zero-initialized accumulator
    };
}

int main() {
    auto int_acc = make_accumulator<int>();
    std::cout << "Int sum: " << int_acc(10)(20)(30) << "\n";

    auto float_acc = make_accumulator<float>();
    std::cout << "Float sum: " << float_acc(1.5f)(2.5f)(3.0f) << "\n";
}

/*
g++ -o accumulator accumulator.cpp -std=c++20 -I ../include -Wall
*/
