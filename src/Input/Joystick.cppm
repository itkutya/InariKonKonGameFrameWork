module;

#include <cstdint>
#include <string_view>

export module Joystick;

import NonConstructible;
import Flag;

export namespace ikk
{
    struct Joystick final : NonConstructible
    {
        using ID = std::uint32_t;

        enum struct [[nodiscard]] Button : std::uint8_t
        {
            Unknown = 0,
            A,
            B,
            X,
            Y,
            LBumper,
            RBumper,
            Back,
            Start,
            Guide,
            LThump,
            RThump,
            Up,
            Down,
            Right,
            Left
        };

        enum struct [[nodiscard]] Axis : std::uint8_t
        {
            Unknown = 0,
            LX,
            LY,
            RX,
            RY,
            LTrigger,
            RTrigger
        };

        struct Hat
        {
            using Index = std::size_t;

            enum struct [[nodiscard]] State : std::uint8_t
            {
                Unknown     = 0 << 0,
                Center      = 1 << 0,
                Up          = 1 << 1,
                Right       = 1 << 2,
                Down        = 1 << 3,
                Left        = 1 << 4,
                UpRight     = Up | Right,
                UpLeft      = Up | Left,
                DownRight   = Down | Right,
                DownLeft    = Down | Left
            };

            Index index = 0;
            Flag<State> state = State::Unknown;

            [[nodiscard]] inline constexpr bool operator==(const Hat& other) const noexcept = default;
            [[nodiscard]] inline constexpr bool operator!=(const Hat& other) const noexcept = default;
        };

        using enum Button;

        inline static constexpr std::size_t ButtonCount = static_cast<std::size_t>(Button::Left);
        inline static constexpr std::size_t AxisCount   = static_cast<std::size_t>(Axis::RTrigger);
        inline static constexpr std::size_t HatCount    = 9;

        [[nodiscard]] static constexpr std::string_view toString(Button button) noexcept;
        [[nodiscard]] static constexpr std::string_view toString(Axis axis) noexcept;
        [[nodiscard]] static constexpr std::string_view toString(Hat::State hat) noexcept;
    };
}

namespace ikk
{
    constexpr std::string_view Joystick::toString(Button button) noexcept
    {
        switch (button)
        {
        case Button::Unknown:   return "Unknown";
        case Button::A:         return "A";
        case Button::B:         return "B";
        case Button::X:         return "X";
        case Button::Y:         return "Y";
        case Button::LBumper:   return "Left Bumper";
        case Button::RBumper:   return "Right Bumper";
        case Button::Back:      return "Back";
        case Button::Start:     return "Start";
        case Button::Guide:     return "Guide";
        case Button::LThump:    return "Left Thump";
        case Button::RThump:    return "Right Thump";
        case Button::Up:        return "DPAD Up";
        case Button::Down:      return "DPAD Down";
        case Button::Right:     return "DPAD Right";
        case Button::Left:      return "DPAD Left";
        }
        return "Unknown";
    }

    constexpr std::string_view Joystick::toString(Axis axis) noexcept
    {
        switch (axis)
        {
        case Axis::Unknown:     return "Unknown";
        case Axis::LX:          return "Left X";
        case Axis::LY:          return "Left Y";
        case Axis::RX:          return "Right X";
        case Axis::RY:          return "Right Y";
        case Axis::LTrigger:    return "Left Trigger";
        case Axis::RTrigger:    return "Right Trigger";
        }
        return "Unknown";
    }

    constexpr std::string_view Joystick::toString(Hat::State hat) noexcept
    {
        switch (hat)
        {
        case Hat::State::Unknown:       return "Unknown";
        case Hat::State::Center:        return "Centered";
        case Hat::State::Up:            return "Up";
        case Hat::State::Right:         return "Right";
        case Hat::State::Down:          return "Down";
        case Hat::State::Left:          return "Left";
        case Hat::State::UpRight:       return "Up & Right";
        case Hat::State::UpLeft:        return "Up & Left";
        case Hat::State::DownRight:     return "Down & Right";
        case Hat::State::DownLeft:      return "Down & Left";
        }
        return "Unknown";
    }
}