module;

#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

export module Color;

import NumericCasts;
import Clamped;
import Number;

export namespace ikk
{
    template<Number T>
    struct [[nodiscard]] BasicColor final
    {
        [[nodiscard]] constexpr BasicColor() noexcept = default;
        [[nodiscard]] constexpr BasicColor(T _r, T _g, T _b, T _a = std::numeric_limits<T>::max()) noexcept;

        constexpr BasicColor(const BasicColor&) noexcept = default;
        constexpr BasicColor(BasicColor&&) noexcept = default;

        constexpr BasicColor& operator=(const BasicColor&) noexcept = default;
        constexpr BasicColor& operator=(BasicColor&&) noexcept = default;

        constexpr ~BasicColor() noexcept = default;

        T r{};
        T g{};
        T b{};
        T a{ std::numeric_limits<T>::max() };

        [[nodiscard]] static constexpr BasicColor White() noexcept;
        [[nodiscard]] static constexpr BasicColor Black() noexcept;
        [[nodiscard]] static constexpr BasicColor Red() noexcept;
        [[nodiscard]] static constexpr BasicColor Green() noexcept;
        [[nodiscard]] static constexpr BasicColor Blue() noexcept;
        [[nodiscard]] static constexpr BasicColor Yellow() noexcept;
        [[nodiscard]] static constexpr BasicColor Magenta() noexcept;
        [[nodiscard]] static constexpr BasicColor Cyan() noexcept;
        [[nodiscard]] static constexpr BasicColor Transparent() noexcept;

        [[nodiscard]] static constexpr BasicColor CornflowerBlue() noexcept;
        [[nodiscard]] static constexpr BasicColor Miku() noexcept;
        [[nodiscard]] static constexpr BasicColor Teto() noexcept;
        [[nodiscard]] static constexpr BasicColor Rin() noexcept;
    };

    using Colorf    = BasicColor<Clamped<float, 0.f, 1.f>>;
    using Coloru8   = BasicColor<std::uint8_t>;
    using Color     = Coloru8;
}

namespace ikk
{
    template<Number T>
    [[nodiscard]] inline consteval T convert(std::uint8_t value) noexcept
    {
        Clamped temp{ value };
        if constexpr (std::is_fundamental_v<T>)
        {
            Clamped<T> temp2{ temp };
            return T{ temp2 };
        }
        else
        {
            Clamped<typename T::Type> temp2{ temp };
            return T{ temp2 };
        }
    }

    template<Number T>
    constexpr BasicColor<T>::BasicColor(T _r, T _g, T _b, T _a) noexcept
        : r(_r), g(_g), b(_b), a(_a)
    {
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::White() noexcept
    {
        return BasicColor{ convert<T>(0xFF), convert<T>(0xFF), convert<T>(0xFF) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Black() noexcept
    {
        return BasicColor{ convert<T>(0x00), convert<T>(0x00), convert<T>(0x00) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Red() noexcept
    {
        return BasicColor{ convert<T>(0xFF), convert<T>(0x00), convert<T>(0x00) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Green() noexcept
    {
        return BasicColor{ convert<T>(0x00), convert<T>(0xFF), convert<T>(0x00) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Blue() noexcept
    {
        return BasicColor{ convert<T>(0x00), convert<T>(0x00), convert<T>(0xFF) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Yellow() noexcept
    {
        return BasicColor{ convert<T>(0xFF), convert<T>(0xFF), convert<T>(0x00) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Magenta() noexcept
    {
        return BasicColor{ convert<T>(0xFF), convert<T>(0x00), convert<T>(0xFF) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Cyan() noexcept
    {
        return BasicColor{ convert<T>(0x00), convert<T>(0xFF), convert<T>(0xFF) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Transparent() noexcept
    {
        return BasicColor{ convert<T>(0x00), convert<T>(0x00), convert<T>(0x00), convert<T>(0x00) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::CornflowerBlue() noexcept
    {
        return BasicColor{ convert<T>(0x64), convert<T>(0x95), convert<T>(0xED) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Miku() noexcept
    {
        return BasicColor{ convert<T>(0x39), convert<T>(0xC5), convert<T>(0xBB) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Teto() noexcept
    {
        return BasicColor{ convert<T>(0xE3), convert<T>(0x42), convert<T>(0x34) };
    }

    template<Number T>
    constexpr BasicColor<T> BasicColor<T>::Rin() noexcept
    {
        return BasicColor{ convert<T>(0xFF), convert<T>(0xD7), convert<T>(0x00) };
    }
}