module;

#include <algorithm>
#include <compare>
#include <cstdint>
#include <limits>
#include <type_traits>

export module Clamped;

import NonConstructible;
import Number;

namespace ikk
{
    template<Number T>
    struct DefaultClampedRange final : NonConstructible
    {
        inline static constexpr T min() noexcept { return std::numeric_limits<T>::min(); }
        inline static constexpr T max() noexcept { return std::numeric_limits<T>::max(); }
    };

    template<>
    struct DefaultClampedRange<float> final : NonConstructible
    {
        inline static constexpr float min() noexcept { return 0.f; }
        inline static constexpr float max() noexcept { return 1.f; }
    };

    template<>
    struct DefaultClampedRange<double> final : NonConstructible
    {
        inline static constexpr double min() noexcept { return 0.0; }
        inline static constexpr double max() noexcept { return 1.0; }
    };

    template<>
    struct DefaultClampedRange<long double> final : NonConstructible
    {
        inline static constexpr long double min() noexcept { return 0.0L; }
        inline static constexpr long double max() noexcept { return 1.0L; }
    };
}

export namespace ikk
{
    template<Number T, T Min = DefaultClampedRange<T>::min(), T Max = DefaultClampedRange<T>::max()>
    class [[nodiscard]] Clamped final
    {
    public:
        using Type = T;

        [[nodiscard]] constexpr Clamped() noexcept = default;
        [[nodiscard]] constexpr Clamped(T value) noexcept;

        constexpr Clamped(const Clamped&) noexcept = default;
        constexpr Clamped(Clamped&&) noexcept = default;

        constexpr Clamped& operator=(const Clamped&) noexcept = default;
        constexpr Clamped& operator=(Clamped&&) noexcept = default;

        template<T OtherMin, T OtherMax>
        explicit constexpr Clamped(const Clamped<T, OtherMin, OtherMax>& other) noexcept;
        template<T OtherMin, T OtherMax>
        explicit constexpr Clamped(Clamped<T, OtherMin, OtherMax>&& other) noexcept;

        template<T OtherMin, T OtherMax>
        constexpr Clamped& operator=(const Clamped<T, OtherMin, OtherMax>& other) noexcept;
        template<T OtherMin, T OtherMax>
        constexpr Clamped& operator=(Clamped<T, OtherMin, OtherMax>&& other) noexcept;

        template<Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
        explicit constexpr Clamped(const Clamped<U, OtherMin, OtherMax>& other) noexcept;
        template<Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
        explicit constexpr Clamped(Clamped<U, OtherMin, OtherMax>&& other) noexcept;

        template<Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
        constexpr Clamped& operator=(const Clamped<U, OtherMin, OtherMax>& other) noexcept;
        template<Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
        constexpr Clamped& operator=(Clamped<U, OtherMin, OtherMax>&& other) noexcept;

        constexpr ~Clamped() noexcept = default;

        [[nodiscard]] constexpr Clamped operator+(const Clamped& other) const noexcept;
        [[nodiscard]] constexpr Clamped operator-(const Clamped& other) const noexcept;
        [[nodiscard]] constexpr Clamped operator*(const Clamped& other) const noexcept;
        [[nodiscard]] constexpr Clamped operator/(const Clamped& other) const noexcept;

        constexpr Clamped& operator+=(const Clamped& other) noexcept;
        constexpr Clamped& operator-=(const Clamped& other) noexcept;
        constexpr Clamped& operator*=(const Clamped& other) noexcept;
        constexpr Clamped& operator/=(const Clamped& other) noexcept;

        [[nodiscard]] constexpr auto operator<=>(const Clamped&) const noexcept = default;

        [[nodiscard]] constexpr Clamped operator+(T value) const noexcept;
        [[nodiscard]] constexpr Clamped operator-(T value) const noexcept;
        [[nodiscard]] constexpr Clamped operator*(T value) const noexcept;
        [[nodiscard]] constexpr Clamped operator/(T value) const noexcept;

        constexpr Clamped& operator+=(T value) noexcept;
        constexpr Clamped& operator-=(T value) noexcept;
        constexpr Clamped& operator*=(T value) noexcept;
        constexpr Clamped& operator/=(T value) noexcept;

        [[nodiscard]] constexpr auto operator<=>(T value) const noexcept;

        [[nodiscard]] constexpr operator T() const noexcept;

        [[nodiscard]] constexpr T value() const noexcept;
    private:
        T m_value = Min;

        template<Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
        constexpr T convertValueFromOtherRange(U otherValue) const noexcept;
    };
}

namespace ikk
{
    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>::Clamped(T value) noexcept
        : m_value(std::clamp(value, Min, Max))
    {
    }

    template<Number T, T Min, T Max>
    template<T OtherMin, T OtherMax>
    constexpr Clamped<T, Min, Max>::Clamped(const Clamped<T, OtherMin, OtherMax>& other) noexcept
        : m_value(convertValueFromOtherRange<T, OtherMin, OtherMax>(other.value()))
    {
    }

    template<Number T, T Min, T Max>
    template<T OtherMin, T OtherMax>
    constexpr Clamped<T, Min, Max>::Clamped(Clamped<T, OtherMin, OtherMax>&& other) noexcept
        : m_value(convertValueFromOtherRange<T, OtherMin, OtherMax>(other.value()))
    {
    }

    template<Number T, T Min, T Max>
    template<T OtherMin, T OtherMax>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator=(const Clamped<T, OtherMin, OtherMax>& other) noexcept
    {
        this->m_value = this->convertValueFromOtherRange<T, OtherMin, OtherMax>(other.value());
        return *this;
    }

    template<Number T, T Min, T Max>
    template<T OtherMin, T OtherMax>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator=(Clamped<T, OtherMin, OtherMax>&& other) noexcept
    {
        this->m_value = this->convertValueFromOtherRange<T, OtherMin, OtherMax>(other.value());
        return *this;
    }

    template <Number T, T Min, T Max>
    template <Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
    constexpr Clamped<T, Min, Max>::Clamped(const Clamped<U, OtherMin, OtherMax>& other) noexcept
        : m_value(convertValueFromOtherRange<U, OtherMin, OtherMax>(other.value()))
    {
    }

    template <Number T, T Min, T Max>
    template <Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
    constexpr Clamped<T, Min, Max>::Clamped(Clamped<U, OtherMin, OtherMax>&& other) noexcept
        : m_value(convertValueFromOtherRange<U, OtherMin, OtherMax>(other.value()))
    {
    }

    template <Number T, T Min, T Max>
    template <Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator=(const Clamped<U, OtherMin, OtherMax>& other) noexcept
    {
        this->m_value = this->convertValueFromOtherRange<U, OtherMin, OtherMax>(other.value());
        return *this;
    }

    template <Number T, T Min, T Max>
    template <Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator=(Clamped<U, OtherMin, OtherMax>&& other) noexcept
    {
        this->m_value = this->convertValueFromOtherRange<U, OtherMin, OtherMax>(other.value());
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator+(const Clamped<T, Min, Max>& other) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out += other;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator-(const Clamped<T, Min, Max>& other) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out -= other;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator*(const Clamped<T, Min, Max>& other) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out *= other;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator/(const Clamped<T, Min, Max>& other) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out /= other;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator+=(const Clamped<T, Min, Max>& other) noexcept
    {
        this->m_value = std::clamp(this->m_value + other.m_value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator-=(const Clamped<T, Min, Max>& other) noexcept
    {
        this->m_value = std::clamp(this->m_value - other.m_value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator*=(const Clamped<T, Min, Max>& other) noexcept
    {
        this->m_value = std::clamp(this->m_value * other.m_value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator/=(const Clamped<T, Min, Max>& other) noexcept
    {
        this->m_value = std::clamp(this->m_value / other.m_value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator+(T value) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out += value;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator-(T value) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out -= value;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator*(T value) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out *= value;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max> Clamped<T, Min, Max>::operator/(T value) const noexcept
    {
        Clamped<T, Min, Max> out = *this;
        out /= value;
        return out;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator+=(T value) noexcept
    {
        this->m_value = std::clamp(this->m_value + value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator-=(T value) noexcept
    {
        this->m_value = std::clamp(this->m_value - value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator*=(T value) noexcept
    {
        this->m_value = std::clamp(this->m_value * value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>& Clamped<T, Min, Max>::operator/=(T value) noexcept
    {
        this->m_value = std::clamp(this->m_value / value, Min, Max);
        return *this;
    }

    template<Number T, T Min, T Max>
    constexpr auto Clamped<T, Min, Max>::operator<=>(T value) const noexcept
    {
        return this->m_value <=> value;
    }

    template <Number T, T Min, T Max>
    constexpr Clamped<T, Min, Max>::operator T() const noexcept
    {
        return this->m_value;
    }

    template<Number T, T Min, T Max>
    constexpr T Clamped<T, Min, Max>::value() const noexcept
    {
        return this->m_value;
    }

    template<Number T, T Min, T Max>
    template<Number U, U OtherMin, U OtherMax> requires (std::is_convertible_v<U, T>)
    constexpr T Clamped<T, Min, Max>::convertValueFromOtherRange(U otherValue) const noexcept
    {
        using CommonType = std::common_type_t<T, U>;
        using CalcType = std::conditional_t<std::is_integral_v<CommonType>,
            std::conditional_t<std::is_unsigned_v<CommonType>, std::uintmax_t, std::intmax_t>, CommonType>;

        constexpr auto otherMin = static_cast<CalcType>(OtherMin);
        constexpr auto otherMax = static_cast<CalcType>(OtherMax);
        constexpr auto thisMin = static_cast<CalcType>(Min);
        constexpr auto thisMax = static_cast<CalcType>(Max);

        constexpr CalcType otherRange = otherMax - otherMin;
        constexpr CalcType thisRange = thisMax - thisMin;

        if constexpr (otherRange == CalcType{0}) return Min;

        const CalcType result = (static_cast<CalcType>(otherValue) - otherMin) * thisRange / otherRange + thisMin;
        return static_cast<T>(std::clamp(result, thisMin, thisMax));
    }
}