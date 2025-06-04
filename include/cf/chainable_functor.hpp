#pragma once

#include <concepts>
#include <functional>
#include <type_traits>

namespace cf {

struct val   {};
struct cval  {};
struct ref   {};
struct cref  {};
struct rvref {};

namespace detail {

// TODO: use c_one_of

template <typename T> struct is_chainable                   : std::false_type {};
template <>           struct is_chainable<val>            : std::true_type  {};
template <>           struct is_chainable<cval>      : std::true_type  {};
template <>           struct is_chainable<ref>        : std::true_type  {};
template <>           struct is_chainable<cref>  : std::true_type  {};
template <>           struct is_chainable<rvref> : std::true_type  {};

template <typename T>
inline constexpr bool is_chainable_v = is_chainable<T>::value;

template <typename T>
concept c_chainable_type_spec = std::default_initializable<T> && is_chainable_v<T>;


template <c_chainable_type_spec C, typename T>
struct chainable_type_traits {
    using type = void;
};

template <typename T>
struct chainable_type_traits<val, T> {
    using type = std::remove_cvref_t<T>;
};

template <typename T>
struct chainable_type_traits<cval, T> {
    using type = const std::remove_cvref_t<T>;
};

template <typename T>
struct chainable_type_traits<ref, T> {
    using type = std::remove_cvref_t<T>&;
};

template <typename T>
struct chainable_type_traits<cref, T> {
    using type = const std::remove_cvref_t<T>&;
};

template <typename T>
struct chainable_type_traits<rvref, T> {
    using type = std::remove_cvref_t<T>&&;
};

template <c_chainable_type_spec C, typename T>
using chainable_type = typename chainable_type_traits<C, T>::type;

} // namespace detail

template <
    typename T,
    detail::c_chainable_type_spec RT = val,
    detail::c_chainable_type_spec AT = val
>
class chainable_functor {
public:
    using type = std::remove_reference_t<T>;
    using return_type = detail::chainable_type<RT, T>;
    using argument_type = detail::chainable_type<AT, T>;
    using function_type = std::function<return_type(argument_type, argument_type)>;

    chainable_functor() = delete;
    chainable_functor(const chainable_functor&) = delete;
    chainable_functor& operator=(const chainable_functor&) = delete;

    chainable_functor(function_type fun) requires (std::default_initializable<T>)
    : _function(fun) {}

    chainable_functor(function_type fun, type&& init_result)
    : _function(fun), _result(std::forward<type>(init_result)) {}

    chainable_functor(chainable_functor&& other)
    : _function(std::move(other._function)), _result(std::move(other._result)) {}

    chainable_functor<T, RT, AT> operator() (argument_type arg) {
        return chainable_functor<T, RT, AT>(
            this->_function,
            this->_function(this->_result, arg)
        );
    }

    return_type get(chainable_functor<T, RT, AT> functor) {
        return std::forward<return_type>(functor._result);
    }

    operator type() const {
        return this->_result;
    }

private:
    function_type _function;
    type _result = type{};
};

} // namespace cf
