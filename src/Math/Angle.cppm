module;

#include <numbers>

export module Angle;

import TypeUtils;
import Number;

export namespace ikk
{
    template<FloatingPointNumber T>
    struct Radian;

    template<FloatingPointNumber T>
    struct [[nodiscard]] Degree final
    {
        using Type = T;

        [[nodiscard]] inline constexpr operator T() const noexcept
        {
            return this->value;
        }

        [[nodiscard]] inline constexpr Radian<T> toRadian() const noexcept
        {
            return Radian<T>{ .value = this->value * (std::numbers::pi_v<T> / static_cast<T>(180.f)) };
        }

        T value = T{};
    };

    using Degreef = Degree<float>;
    using Degreed = Degree<double>;

    template<FloatingPointNumber T>
    struct [[nodiscard]] Radian final
    {
        using Type = T;

        [[nodiscard]] explicit inline constexpr operator T() const noexcept { return this->value; }

        [[nodiscard]] inline constexpr Degree<T> toDegree() const noexcept
        {
            return Degree<T>{ .value = this->value * (static_cast<T>(180.f) / std::numbers::pi_v<T>) };
        }

        T value = {};
    };

    using Radianf = Radian<float>;
    using Radiand = Radian<double>;

    template<class T>
    concept AngleTypes = Is<T>::template SpecializationOf_v<Degree> || Is<T>::template SpecializationOf_v<Radian>;
}

export [[nodiscard]] inline constexpr ikk::Degreef operator""_deg(long double value) noexcept
{
    return ikk::Degreef{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Radianf operator""_rad(long double value) noexcept
{
    return ikk::Radianf{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Degreef operator""_degf(long double value) noexcept
{
    return ikk::Degreef{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Radianf operator""_radf(long double value) noexcept
{
    return ikk::Radianf{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Degreed operator""_degd(long double value) noexcept
{
    return ikk::Degreed{ .value = static_cast<double>(value) };
}

export [[nodiscard]] inline constexpr ikk::Radiand operator""_radd(long double value) noexcept
{
    return ikk::Radiand{ .value = static_cast<double>(value) };
}

export [[nodiscard]] inline constexpr ikk::Degreef operator""_deg(unsigned long long value) noexcept
{
    return ikk::Degreef{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Radianf operator""_rad(unsigned long long value) noexcept
{
    return ikk::Radianf{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Degreef operator""_degf(unsigned long long value) noexcept
{
    return ikk::Degreef{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Radianf operator""_radf(unsigned long long value) noexcept
{
    return ikk::Radianf{ .value = static_cast<float>(value) };
}

export [[nodiscard]] inline constexpr ikk::Degreed operator""_degd(unsigned long long value) noexcept
{
    return ikk::Degreed{ .value = static_cast<double>(value) };
}

export [[nodiscard]] inline constexpr ikk::Radiand operator""_radd(unsigned long long value) noexcept
{
    return ikk::Radiand{ .value = static_cast<double>(value) };
}