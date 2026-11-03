module;

#include <cmath>
#include <limits>
#include <type_traits>

export module Number;

export namespace ikk
{
    template<class T>
    concept BasicNumber = std::is_arithmetic_v<T> == true && std::is_same_v<T, bool> == false;

    template<class T>
    concept Number = BasicNumber<T> || BasicNumber<typename T::Type>;

    template<class T>
    concept FloatingPointNumber = std::is_floating_point_v<T>;

    template<Number T>
    [[nodiscard]] inline constexpr bool isZero(T value, T tolerance = std::numeric_limits<T>::epsilon()) noexcept
    {
        if constexpr (std::is_floating_point_v<T>) return std::abs(value) <= tolerance;
        else return value == T{0};
    }
}