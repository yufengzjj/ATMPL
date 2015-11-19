#pragma once
#include <tuple>
#include <type_traits>

namespace functional_helper
{
    template <typename F>
    struct function_traits;

    template <typename R, typename... Args>
    struct function_traits<R(Args ...)>
    {
        using return_type = R;

        typedef std::integral_constant<size_t, sizeof...(Args)> arity;

        template <std::size_t N>
        struct arg
        {
            static_assert(N < arity::value, "invalid parameter index.");
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    //function ptr
    template <typename R, typename... Args>
    struct function_traits<R(*)(Args ...)> : public function_traits<R(Args ...)>
    {
    };

    // member function pointer
    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args ...)> : public function_traits<R(C&, Args ...)>
    {
    };

    // const member function pointer
    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args ...) const> : public function_traits<R(C&, Args ...)>
    {
    };

    // member object pointer
    template <typename C, typename R>
    struct function_traits<R(C::*)> : public function_traits<R(C&)>
    {
    };


    // lambda and functor: use decltype of its operator()
    template <typename F>
    struct function_traits
    {
    private:
        using call_type = function_traits<decltype(&F::operator())>;
    public:
        using return_type = typename call_type::return_type;
        // special case arity for function functional_helper using operator()
        typedef std::integral_constant<size_t, call_type::arity::value - 1> arity;

        template <std::size_t N>
        struct arg
        {
            static_assert(N < arity::value, "error: invalid parameter index.");
            using type = typename call_type::template arg<N + 1>::type;
        };
    };

    template <typename F>
    struct function_traits<const F> : public function_traits<F>
    {
    };

    // strip off &'s to avoid compile errors
    template <typename F>
    struct function_traits<F&> : public function_traits<F>
    {
    };

    template <typename F>
    struct function_traits<F&&> : public function_traits<F>
    {
    };


    template <typename F>
    struct is_unary
        : std::integral_constant<bool, function_traits<F>::arity::value == 1>
    {
    };

    template <typename F>
    struct is_binary
        : std::integral_constant<bool, function_traits<F>::arity::value == 2>
    {
    };

    template <typename F>
    struct is_ternary
        : std::integral_constant<bool, function_traits<F>::arity::value == 3>
    {
    };

    template <typename F>
    struct codomain
    {
        using type = typename function_traits<F>::return_type;
    };

    template <typename F>
    struct unary_domain
    {
        static_assert(is_unary<F>::value, "not unary");
        using type = typename function_traits<F>::template arg<0>::type;
    };

    template <typename T>
    struct remove_cvr
    {
        using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    };

    // recursive case
    template <size_t idx, template<size_t> class args>
    struct is_homogeneous_helper
        : std::integral_constant<bool, std::is_same
        <typename remove_cvr<typename args<idx - 1>::type>::type,
        typename remove_cvr<typename args<idx>::type>::type>::value
        && is_homogeneous_helper<idx - 1, args>::value>
    {
        // TODO:
        static_assert(idx > 0, "is_homogeneous can only be applied to procedures with arity::value > 0");
    };

    // base case
    template <template<size_t> class args>
    struct is_homogeneous_helper<0, args> : std::true_type
    {
    };

    template <typename F>
    struct is_homogeneous : std::integral_constant
        <bool, is_homogeneous_helper
        <function_traits<F>::arity::value - 1,
        function_traits<F>::template arg>::value>
    {
        // TODO:
        static_assert(function_traits<F>::arity::value > 0, "is_homogeneous can only be applied to procedures with arity::value > 0");
    };


    template <typename F>
    struct is_predicate
        : std::integral_constant<bool, std::is_same<typename function_traits<F>::return_type, bool>::value>
    {
    };

    template <typename F>
    struct is_homogeneous_predicate
        : std::integral_constant<bool, is_homogeneous<F>::value
        && is_predicate<F>::value>
    {
    };

    template <typename F>
    struct is_unary_predicate
        : std::integral_constant<bool, function_traits<F>::arity::value == 1
        && is_predicate<F>::value>
    {
    };

    // TODO: remove_cvr problematic?
    template <typename F>
    struct is_operation
        : std::integral_constant<bool,
        std::is_same<typename remove_cvr<typename codomain<F>::type>::type,
        typename remove_cvr<typename function_traits<F>::template arg<0>::type>::type>::value
        && is_homogeneous<F>::value>
    {
    };

    template <typename F>
    struct is_transformation // == unary_operation
        : std::integral_constant<bool, is_operation<F>::value
        && is_unary<F>::value>
    {
    };

    template <typename F>
    struct binary_operation
        : std::integral_constant<bool, is_operation<F>::value
        && is_binary<F>::value>
    {
    };

    /*
    relation R:
    strict := R(a, a) == false, for all a
    reflexive := R(a, a) == true, for all a

    symmetric := R(a, b) == R(b, a), for all a, b
    asymmetric := R(a, b) != R(b, a), for all a, b

    transitive := R(a, b) && R(b, c) == R(a, c), for all a, b, c

    equivalence := transitive && reflexive && symmetric

    example for not strict, not reflexive relation:
    bool product_is_even(int x, int y);
    -> reflexive for even numbers
    -> strict for odd numbers
    -> neither of them for the set of natural numbers

    */

    template <typename F>
    struct is_relation
        : std::integral_constant<bool, is_homogeneous_predicate<F>::value
        && function_traits<F>::arity::value == 2>
    {
    };

    template <typename Func, typename Sig>
    struct function_exists
        : std::integral_constant<bool, std::is_same<Func, Sig>::value>
    {
    };
}


