#include <cf/chainable_functor.hpp>

#include <iostream>
#include <string>

int main() {
    std::string separator = " | ";

    cf::chainable_functor<std::string, cf::val, cf::cref> joiner{
        [sep = std::move(separator)](const std::string& a, const std::string& b) {
            return a.empty() ? b : a + sep + b;
        },
        ""
    };

    std::string result = joiner("apple")("banana")("cherry");
    std::cout << "Joined: " << result << "\n";
}

/*
g++ -o str_joiner str_joiner.cpp -std=c++20 -I ../include -Wall
*/
