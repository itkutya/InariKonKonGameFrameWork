module;

#include <cstdint>
#include <string_view>

export module Mouse;

import NonConstructible;

export namespace ikk
{
    struct Mouse final : NonConstructible
    {
        enum struct [[nodiscard]] Button : std::uint8_t
        {
            Unknown = 0,
            Left,
            Right,
            Middle,
            Extra1,
            Extra2,
            Extra3,
            Extra4,
            Extra5,
            LeftButton      = Left,
            RightButton     = Right,
            MiddleButton    = Middle,
            ExtraButton1    = Extra1,
            ExtraButton2    = Extra2,
            ExtraButton3    = Extra3,
            ExtraButton4    = Extra4,
            ExtraButton5    = Extra5
        };

        enum struct [[nodiscard]] Wheel : std::uint8_t
        {
            Unknown = 0,
            Horizontal,
            Vertical,
            HorizontalWheel = Horizontal,
            VerticalWheel   = Vertical
        };

        using enum Button;

        inline static constexpr std::size_t ButtonCount = static_cast<std::size_t>(Button::Extra5);
        inline static constexpr std::size_t WheelCount  = static_cast<std::size_t>(Wheel::Vertical);

        [[nodiscard]] static constexpr std::string_view toString(Button button) noexcept;
        [[nodiscard]] static constexpr std::string_view toString(Wheel wheel) noexcept;
    };
}

namespace ikk
{
    constexpr std::string_view Mouse::toString(Button button) noexcept
    {
        switch (button)
        {
            case Button::Unknown:   return "Unknown";
            case Button::Left:      return "Left";
            case Button::Right:     return "Right";
            case Button::Middle:    return "Middle";
            case Button::Extra1:    return "Extra 1";
            case Button::Extra2:    return "Extra 2";
            case Button::Extra3:    return "Extra 3";
            case Button::Extra4:    return "Extra 4";
            case Button::Extra5:    return "Extra 5";
        }
        return "Unknown";
    }

    constexpr std::string_view Mouse::toString(Wheel wheel) noexcept
    {
        switch (wheel)
        {
            case Wheel::Unknown:    return "Unknown";
            case Wheel::Vertical:   return "Vertical";
            case Wheel::Horizontal: return "Horizontal";
        }
        return "Unknown";
    }
}