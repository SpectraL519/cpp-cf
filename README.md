# cpp-cf
Chainable functor class for C++20

> The `ChainableFunctor` class allows chaining of **non-void two-argument functions** the  return and argument types of which are (const) value, (const) reference or rvalue reference types.
>
> **NOTE:** Return and argument types have to be the same regardless of the const specifier and the function signature is represented as `std::function<ReturnType(ArgumentType, AgrymentType)>`



<br />
<br />

## Quick Start

1. **Adding `cpp-cf` to a project**

    **TODO**

<br />
<br />

2. **Using the `ChainableFunctor` class in a project**

    To be able to use the `ChainableFunctor` class you simply need to include a single header file:

    ```
    #include <cf/chainable_functor.hpp>
    ```

<br />
<br />

3. **Initializing a `ChainableFunctor` object**

    The `ChainableFunctor` class requires specyfying the type you want to wark with as well as the function return (`RT`) and argument (`AT`) types:

    **NOTE:** function return and argument types are `cf::val` by default, bu you can chose from:
    * `cf::val`
    * `cf::cval`
    * `cf::ref`
    * `cf::cref`
    * `cf::rvref`

    You also need to pass in a function of a specified signature to the class contructor.

    Optionally you can pass in a initial result as well (if not it will be initialized to the default value of the given type) - this is required for types which are not default initializable.

    **Examples:**

    ```
    cf::ChainableFunctor<int> adder{[](int x, int y) { return x + y; }};
    ```

    ```
    cf::ChainableFunctor<std::unique_ptr<int>, cf::val, cf::cref> ptrAdder{
        [](const std::unique_ptr<int>& a, const std::unique_ptr<int>& b) {
            return std::make_unique<int>(*a + *b);
        },
        std::make_unique<int>()
    };
    ```

<br />
<br />

4. **Chaining function calls**

    To properly chain the function calls you simply need to call the `ChainableFunctor` object as many times as you want (or as many times as your hardware allows you to :wink:)

    To get the result of the chained function call you can:

    * Implicitly convert the returned functor to your desired type (returns the stored result by value)

    ```
    int chainResult = adder(1)(2)(3);
    std::cout << "Chain result: " << chainResult << std::endl;
    ```

    * Use the `ChainableFunctor<T, RT, AT>::get()` method (forwards the stored result)

    ```
    int chainResultFromFunc = adder.get(adder(1)(2)(3)(4));
    std::cout << "Chain result (from function): " << chainResultFromFunc << std::endl;
    ```

<br />
<br />

5. **An example program**

    Here is a very simple example program which demonstrates the usage of the `ChainableFunctor` class:

    ```
    // example.cpp

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
    ```

    ```
    ./example
    Single argument result: 1
    Chain result: 10
    Chain result (from function): 15

    Ptr chain result: 6
    ```

<br />
<br />
<br />

## TODO:

* Figure out how to chain function with different argument types
  > Maybe use std::function<ReturnType(ReturnType, ArgumentType)>

* Unary function chaining (or use variadic number of function arguments)

* Add `()` operator overload for `ArgumentType... args`?
