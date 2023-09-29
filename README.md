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

    **NOTE:** function return and argument types are `cf::value` by default, bu you can chose from:
    * `cf::value`
    * `cf::const_value`
    * `cf::reference`
    * `cf::const_reference`
    * `cf::rvalue_reference`

    You also need to pass in a function of a specified signature to the class contructor.

    Optionally you can pass in a initial result as well (if not it will be initialized to the default value of the given type) - this is required for types which are not default initializable.

    **Examples:**

    ```
    cf::ChainableFunctor<int> adder{[](int x, int y) { return x + y; }};
    ```

    ```
    cf::ChainableFunctor<std::unique_ptr<int>, cf::value, cf::const_reference> ptrAdder{
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

    * Use the `ChainableFunctor<T, RT, AT>::result()` method (forwards the stored result)

    ```
    int chainResultFromFunc = adder.result(adder(1)(2)(3)(4));
    std::cout << "Chain result (from function): " << chainResultFromFunc << std::endl;
    ```

<br />
<br />

5. **An example program**

    Here is a very simple example program which demonstrates the usage of the `ChainableFunctor` class:

    ```
    // example.cpp

    #include <iostream>
    #include <memory>

    #include <cf/chainable_functor.hpp>



    int main(void) {
        cf::ChainableFunctor<int> adder{[](int x, int y) { return x + y; }};

        int singelArgResult = adder(5);
        std::cout << "Single argument result: " << singelArgResult << std::endl;

        int chainResult = adder(5)(6)(7)(8)(9);
        std::cout << "Chain result: " << chainResult << std::endl;

        int chainResultFromFunc = adder.result(adder(5)(6)(7)(8)(9)(10));
        std::cout << "Chain result (from function): " << chainResultFromFunc << std::endl;


        cf::ChainableFunctor<std::unique_ptr<int>, cf::value, cf::const_reference> ptrAdder{
            [](const std::unique_ptr<int>& a, const std::unique_ptr<int>& b) {
                return std::make_unique<int>(*a + *b);
            },
            std::make_unique<int>()
        };

        auto aPtr = std::make_unique<int>(1);
        auto bPtr = std::make_unique<int>(2);
        auto cPtr = std::make_unique<int>(3);

        std::cout << "Ptr chain result: "
                << *ptrAdder.result(ptrAdder(aPtr)(bPtr)(cPtr)) << std::endl;

        return 0;
    }
    ```

<br />
<br />
<br />

## TODO:

* Figure out how to chain function with different argument types
  > Maybe use std::function<ReturnType(ReturnType, ArgumentType)>

* Add `()` operator overload for `ArgumentType... args`?
