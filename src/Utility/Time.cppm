module;

#include <chrono>

export module Time;

import Number;

export namespace ikk
{
    class [[nodiscard]] Time final
    {
    public:
        [[nodiscard]] constexpr Time() noexcept = default;
        template<class Rep, class Period>
        [[nodiscard]] explicit constexpr Time(const std::chrono::duration<Rep, Period>& duration) noexcept;

        constexpr Time(const Time&) noexcept = default;
        constexpr Time(Time&&) noexcept = default;

        constexpr Time& operator=(const Time&) noexcept = default;
        constexpr Time& operator=(Time&&) noexcept = default;

        constexpr ~Time() noexcept = default;

        [[nodiscard]] constexpr auto asSeconds() const noexcept;
        [[nodiscard]] constexpr auto asMilliseconds() const noexcept;
        [[nodiscard]] constexpr auto asMicroseconds() const noexcept;

        template<class Duration>
        [[nodiscard]] constexpr Duration toDuration() const noexcept;

        [[nodiscard]] constexpr Time operator+() const noexcept;
        [[nodiscard]] constexpr Time operator-() const noexcept;

        constexpr Time& operator+=(const Time& other) noexcept;
        constexpr Time& operator-=(const Time& other) noexcept;

        [[nodiscard]] constexpr Time operator+(const Time& other) const noexcept;
        [[nodiscard]] constexpr Time operator-(const Time& other) const noexcept;

        constexpr Time& operator*=(BasicNumber auto scale) noexcept;
        constexpr Time& operator/=(BasicNumber auto scale) noexcept;

        [[nodiscard]] constexpr Time operator*(BasicNumber auto scale) const noexcept;
        [[nodiscard]] constexpr Time operator/(BasicNumber auto scale) const noexcept;

        [[nodiscard]] constexpr auto operator<=>(const Time& other) const noexcept = default;
    private:
        std::chrono::nanoseconds m_value{};
    };

    [[nodiscard]] inline constexpr Time seconds(BasicNumber auto amount) noexcept
    {
        using ReturnType = std::chrono::seconds;
        using Rep = ReturnType::rep;
        return Time{ReturnType(static_cast<Rep>(amount))};
    }

    [[nodiscard]] inline constexpr Time milliseconds(BasicNumber auto amount) noexcept
    {
        using ReturnType = std::chrono::milliseconds;
        using Rep = ReturnType::rep;
        return Time{ReturnType(static_cast<Rep>(amount))};
    }

    [[nodiscard]] inline constexpr Time microseconds(BasicNumber auto amount) noexcept
    {
        using ReturnType = std::chrono::microseconds;
        using Rep = ReturnType::rep;
        return Time{ReturnType(static_cast<Rep>(amount))};
    }
}

namespace ikk
{
    template<class Rep, class Period>
    constexpr Time::Time(const std::chrono::duration<Rep, Period>& duration) noexcept
        : m_value(std::chrono::duration_cast<std::chrono::nanoseconds>(duration))
    {
    }

    constexpr auto Time::asSeconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::seconds>(this->m_value);
    }

    constexpr auto Time::asMilliseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(this->m_value);
    }

    constexpr auto Time::asMicroseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(this->m_value);
    }

    template<class Duration>
    constexpr Duration Time::toDuration() const noexcept
    {
        return std::chrono::duration_cast<Duration>(this->m_value);
    }

    constexpr Time Time::operator+() const noexcept
    {
        return *this;
    }

    constexpr Time Time::operator-() const noexcept
    {
        return Time{-this->m_value};
    }

    constexpr Time& Time::operator+=(const Time& other) noexcept
    {
        this->m_value += other.m_value;
        return *this;
    }

    constexpr Time& Time::operator-=(const Time& other) noexcept
    {
        this->m_value -= other.m_value;
        return *this;
    }

    constexpr Time Time::operator+(const Time& other) const noexcept
    {
        Time out = *this;
        out += other;
        return out;
    }

    constexpr Time Time::operator-(const Time& other) const noexcept
    {
        Time out = *this;
        out -= other;
        return out;
    }

    constexpr Time& Time::operator*=(BasicNumber auto scale) noexcept
    {
        this->m_value *= static_cast<std::chrono::nanoseconds::rep>(scale);
        return *this;
    }

    constexpr Time& Time::operator/=(BasicNumber auto scale) noexcept
    {
        this->m_value /= static_cast<std::chrono::nanoseconds::rep>(scale);
        return *this;
    }

    constexpr Time Time::operator*(BasicNumber auto scale) const noexcept
    {
        Time out = *this;
        out *= scale;
        return out;
    }

    constexpr Time Time::operator/(BasicNumber auto scale) const noexcept
    {
        Time out = *this;
        out /= scale;
        return out;
    }
}

export [[nodiscard]] inline constexpr auto operator""_ns(unsigned long long value) noexcept
{
    return ikk::Time{std::chrono::duration<decltype(value), std::nano>{value}};
}

export [[nodiscard]] inline constexpr auto operator""_us(unsigned long long value) noexcept
{
    return ikk::microseconds(value);
}

export [[nodiscard]] inline constexpr auto operator""_ms(unsigned long long value) noexcept
{
    return ikk::milliseconds(value);
}

export [[nodiscard]] inline constexpr auto operator""_s(unsigned long long value) noexcept
{
    return ikk::seconds(value);
}

export [[nodiscard]] inline constexpr auto operator""_min(unsigned long long value) noexcept
{
    return ikk::Time{std::chrono::duration<decltype(value), std::chrono::minutes::period>{value}};
}

export [[nodiscard]] inline constexpr auto operator""_h(unsigned long long value) noexcept
{
    return ikk::Time{std::chrono::duration<decltype(value), std::chrono::hours::period>{value}};
}

export [[nodiscard]] inline constexpr auto operator""_ns(long double value) noexcept
{
    return ikk::Time{std::chrono::duration<decltype(value), std::nano>{value}};
}

export [[nodiscard]] inline constexpr auto operator""_us(long double value) noexcept
{
    return ikk::microseconds(value);
}

export [[nodiscard]] inline constexpr auto operator""_ms(long double value) noexcept
{
    return ikk::milliseconds(value);
}

export [[nodiscard]] inline constexpr auto operator""_s(long double value) noexcept
{
    return ikk::seconds(value);
}

export [[nodiscard]] inline constexpr auto operator""_min(long double value) noexcept
{
    return ikk::Time{std::chrono::duration<decltype(value), std::chrono::minutes::period>{value}};
}

export [[nodiscard]] inline constexpr auto operator""_h(long double value) noexcept
{
    return ikk::Time{std::chrono::duration<decltype(value), std::chrono::hours::period>{value}};
}