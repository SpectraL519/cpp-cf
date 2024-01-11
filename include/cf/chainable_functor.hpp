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

template <typename T, typename... ValidTypes>
inline constexpr bool is_valid_type_v = std::disjunction_v<std::is_same<T, ValidTypes...>>;

template <typename T>
inline constexpr bool is_chainable_v = is_valid_type_v<T, value, const_value, reference, const_reference, rvalue_reference>;

template <typename T>
concept chainable_type = std::default_initializable<T> && is_chainable_v<T>;

template <chainable_type C, typename T>
struct chainable_type_traits {
    using type = void;
};

template <typename T>
struct chainable_type_traits<value, T> {
    // TODO: std::remove_cvref_t
    using type = std::remove_reference_t<T>;
};

template <typename T>
struct chainable_type_traits<const_value, T> {
    // TODO: std::remove_cvref_t
    using type = const std::remove_reference_t<T>;
};

template <typename T>
struct chainable_type_traits<reference, T> {
    // TODO: std::remove_cvref_t
    using type = std::remove_reference_t<T>&;
};

template <typename T>
struct chainable_type_traits<const_reference, T> {
    // TODO: std::remove_cvref_t
    using type = const std::remove_reference_t<T>&;
};

template <typename T>
struct chainable_type_traits<rvalue_reference, T> {
    // TODO: std::remove_cvref_t
    using type = std::remove_reference_t<T>&&;
};

template <chainable_type C, typename T>
using chainable_type_traits_t = typename chainable_type_traits<C, T>::type;

} // namespace detail



template <
    typename T,
    detail::chainable_type RT = value,
    detail::chainable_type AT = value
>
class ChainableFunctor {
public:
    using type = std::remove_reference_t<T>;
    using return_type = detail::chainable_type_traits_t<RT, T>;
    using argument_type = detail::chainable_type_traits_t<AT, T>;
    using function_type = std::function<return_type(argument_type, argument_type)>;

    ChainableFunctor() = delete;
    ChainableFunctor(const ChainableFunctor&) = delete;
    ChainableFunctor& operator=(const ChainableFunctor&) = delete;

    ChainableFunctor(function_type fun) requires (std::default_initializable<T>)
    : _function(fun) {}

    ChainableFunctor(function_type fun, type&& init_result)
    : _function(fun), _result(std::forward<type>(init_result)) {}

    ChainableFunctor(ChainableFunctor&& other)
    : _function(std::move(other._function)), _result(std::move(other._result)) {}

    ChainableFunctor<T, RT, AT> operator() (argument_type arg) {
        return ChainableFunctor<T, RT, AT>(
            this->_function,
            this->_function(this->_result, arg)
        );
    }

    return_type result(ChainableFunctor<T, RT, AT> functor) {
        return std::forward<return_type>(functor._result);
    }

    operator type() const {
        return this->_result;
    }

private:
    // TODO: std::shared_ptr<function_type>
    function_type _function;
    type _result = type{};
};

} // namespace cf
