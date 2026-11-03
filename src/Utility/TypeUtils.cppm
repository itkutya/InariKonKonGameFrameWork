module;

#include <concepts>
#include <tuple>
#include <type_traits>
#include <variant>

export module TypeUtils;

import NonConstructible;

namespace ikk
{
    template<class, template<class...> class>
    struct IsSpecializationOf : std::false_type
    {
    };

    template<template<class...> class Template, class... Args>
    struct IsSpecializationOf<Template<Args...>, Template> : std::true_type
    {
    };

    template<class, class>
    struct IsAlternativeOf : std::false_type
    {
    };

    template<class T, class... Types>
    struct IsAlternativeOf<T, std::variant<Types...>>  : std::bool_constant<(std::same_as<T, Types> || ...)>
    {
    };

    template<class...>
    struct Unique : std::true_type
    {
    };

    template<class T, class... Rest>
    struct Unique<T, Rest...> : std::bool_constant<(!std::is_same_v<T, Rest> && ...) && Unique<Rest...>::value>
    {
    };
}

export namespace ikk
{
    template<class T>
    using TypeIdentity = std::type_identity<T>;

    template<class... Args>
    struct Are final : NonConstructible
    {
        using UniqueTypes = Unique<Args...>;

        inline static constexpr bool UniqueTypes_v = UniqueTypes::value;
    };

    template<class T>
    struct Is final : NonConstructible
    {
        inline static constexpr bool Callable_v = requires { &std::remove_cvref_t<T>::operator(); };

        template<template<class...> class Template>
        using SpecializationOf = IsSpecializationOf<std::remove_cvref_t<T>, Template>;

        template<template<class...> class Template>
        inline static constexpr bool SpecializationOf_v = SpecializationOf<Template>::value;

        template<class Variant>
        using AlternativeOf = IsAlternativeOf<std::remove_cvref_t<T>, Variant>;

        template<class Variant>
        inline static constexpr bool AlternativeOf_v = AlternativeOf<Variant>::value;
    };

    template<class T>
    concept Callable = Is<T>::Callable_v;

    template<class>
    struct FunctionTraits;

    template<Callable T>
    struct FunctionTraits<T> final : FunctionTraits<decltype(&std::remove_cvref_t<T>::operator())>
    {
    };

    template<class C, class R, class... Args>
    struct FunctionTraits<R(C::*)(Args...) const noexcept>
    {
        using Class = C;
        using ReturnType = R;
        using ArgsTuple = std::tuple<Args...>;

        inline static constexpr bool IsConst = true;
        inline static constexpr bool IsNoexcept = true;
    };

    template<class C, class R, class... Args>
    struct FunctionTraits<R(C::*)(Args...) noexcept>
    {
        using Class = C;
        using ReturnType = R;
        using ArgsTuple = std::tuple<Args...>;

        inline static constexpr bool IsConst = false;
        inline static constexpr bool IsNoexcept = true;
    };

    template<class C, class R, class... Args>
    struct FunctionTraits<R(C::*)(Args...) const>
    {
        using Class = C;
        using ReturnType = R;
        using ArgsTuple = std::tuple<Args...>;

        inline static constexpr bool IsConst = true;
        inline static constexpr bool IsNoexcept = false;
    };

    template<class C, class R, class... Args>
    struct FunctionTraits<R(C::*)(Args...)>
    {
        using Class = C;
        using ReturnType = R;
        using ArgsTuple = std::tuple<Args...>;

        inline static constexpr bool IsConst = false;
        inline static constexpr bool IsNoexcept = false;
    };
}