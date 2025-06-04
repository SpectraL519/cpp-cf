#include <cf/chainable_functor.hpp>

#include <iostream>
#include <memory>

int main() {
    cf::chainable_functor<int> adder{
        [](int x, int y) { return x + y; }
    };

    int single = adder(1);
    std::cout << "Single: " << single << "\n";

    int chain = adder(1)(2)(3)(4);
    std::cout << "Chain: " << chain << "\n";

    int total = adder.get(adder(1)(2)(3)(4)(5));
    std::cout << "Total (via .get()): " << total << "\n";

    cf::chainable_functor<std::unique_ptr<int>, cf::val, cf::cref> ptr_adder{
        [](const std::unique_ptr<int>& a, const std::unique_ptr<int>& b) {
            return std::make_unique<int>(*a + *b);
        },
        std::make_unique<int>(0)
    };

    auto a = std::make_unique<int>(1);
    auto b = std::make_unique<int>(2);
    auto c = std::make_unique<int>(3);

    std::cout << "Ptr chain: " << *ptr_adder.get(ptr_adder(a)(b)(c)) << "\n";
}

/*
g++ -o adder adder.cpp -std=c++20 -I ../include -Wall
*/
