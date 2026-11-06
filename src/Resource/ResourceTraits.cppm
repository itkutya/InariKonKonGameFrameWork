module;

#include <concepts>
#include <cstdint>
#include <filesystem>
#include <tuple>
#include <type_traits>
#include <variant>

export module ResourceTraits;

export namespace ikk
{
    template<class T>
    struct ResourceTraits;

    // Example:
    // template<>
    // struct ResourceTraits<TestResource>
    // {
    //     static constexpr bool defaultConstructible = true;
    //
    //     using Constructors = std::variant
    //     <
    //         std::tuple<>,       // Texture(path)
    //         std::tuple<int>     // Texture(path, int)
    //     >;
    // };
}

namespace ikk
{
    template<class T, class Tuple>
    struct IsResourceConstructible;

    template<class T, class... Args>
    struct IsResourceConstructible<T, std::tuple<Args...>>
        : std::bool_constant<std::constructible_from<T, const std::filesystem::path&, Args...>>
    {};

    template<class T, class... Tuples>
    struct IsResourceConstructible<T, std::variant<Tuples...>>
        : std::bool_constant<(IsResourceConstructible<T, Tuples>::value && ...)>
    {};
}

export namespace ikk
{
    enum struct LoadingMode : std::uint8_t
    {
        Immediate,
        Async,
        OnDemand,
        Deferred = OnDemand
    };

    enum struct LoadingState : std::uint8_t
    {
        Unloaded,
        Loading,
        Loaded,
        Failed
    };

    template<class T>
    concept ResourceType = requires
    {
        typename ResourceTraits<T>::Constructors;
        { ResourceTraits<T>::defaultConstructible } -> std::convertible_to<bool>;
    } &&
    IsResourceConstructible<T, typename ResourceTraits<T>::Constructors>::value &&
    (
        ResourceTraits<T>::defaultConstructible == false || std::default_initializable<T> == true
    );
}