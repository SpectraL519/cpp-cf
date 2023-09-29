#pragma once

#include <concepts>
#include <functional>
#include <type_traits>



namespace cf {

struct value            {};
struct const_value      {};
struct reference        {};
struct const_reference  {};
struct rvalue_reference {};

namespace detail {

template <typename T> struct is_chainable                   : std::false_type {};
template <>           struct is_chainable<value>            : std::true_type  {};
template <>           struct is_chainable<const_value>      : std::true_type  {};
template <>           struct is_chainable<reference>        : std::true_type  {};
template <>           struct is_chainable<const_reference>  : std::true_type  {};
template <>           struct is_chainable<rvalue_reference> : std::true_type  {};

template <typename T>
inline constexpr bool is_chainable_v = is_chainable<T>::value;

template <typename T>
concept chainable_type = std::default_initializable<T> && is_chainable_v<T>;


template <chainable_type C, typename T>
struct chainable_type_traits {
    using type = void;
};

template <typename T>
struct chainable_type_traits<value, T> {
    using type = std::remove_reference_t<T>;
};

template <typename T>
struct chainable_type_traits<const_value, T> {
    using type = const std::remove_reference_t<T>;
};

template <typename T>
struct chainable_type_traits<reference, T> {
    using type = std::remove_reference_t<T>&;
};

template <typename T>
struct chainable_type_traits<const_reference, T> {
    using type = const std::remove_reference_t<T>&;
};

template <typename T>
struct chainable_type_traits<rvalue_reference, T> {
    using type = std::remove_reference_t<T>&&;
};

template <chainable_type C, typename T>
using chainable_type_traits_t = typename chainable_type_traits<C, T>::type;

} // namespace detail



template <
    std::default_initializable T,
    detail::chainable_type RT = value,
    detail::chainable_type AT = value
>
class ChainableFunctor {
public:
    using Type = std::remove_reference_t<T>;
    using ReturnType = detail::chainable_type_traits_t<RT, T>;
    using ArgumentType = detail::chainable_type_traits_t<AT, T>;
    using FunctionType = std::function<ReturnType(ArgumentType, ArgumentType)>;

    ChainableFunctor() = delete;
    ChainableFunctor(const ChainableFunctor&) = delete;
    ChainableFunctor& operator=(const ChainableFunctor&) = delete;

    ChainableFunctor(FunctionType fun)
    : _function(fun) {}

    ChainableFunctor(FunctionType fun, Type&& init_result)
    : _function(fun), _result(std::forward<Type>(init_result)) {}

    ChainableFunctor(ChainableFunctor&& other)
    : _function(std::move(other._function)), _result(std::move(other._result)) {}

    ChainableFunctor<T, RT, AT> operator() (ArgumentType arg) {
        return ChainableFunctor<T, RT, AT>(
            this->_function,
            this->_function(this->_result, arg)
        );
    }

    ReturnType result(ChainableFunctor<T, RT, AT> functor) {
        return std::forward<ReturnType>(functor._result);
    }

    operator Type() const {
        return this->_result;
    }

private:
    FunctionType _function;
    Type _result = Type{};
};

} // namespace cf
