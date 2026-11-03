module;

#include <cstdint>
#include <string_view>

export module Keyboard;

import NonConstructible;

export namespace ikk
{
    struct Keyboard final : NonConstructible
    {
        using ScanCode = std::int32_t;

        enum struct [[nodiscard]] KeyCode : std::uint8_t
        {
            Unknown = 0,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,
            Num0,
            Enter,
            Escape,
            Backspace,
            Tab,
            Space,
            Hyphen,
            Equal,
            LBracket,
            RBracket,
            Backslash,
            Semicolon,
            Apostrophe,
            Grave,
            Comma,
            Period,
            Slash,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,
            CapsLock,
            PrintScreen,
            ScrollLock,
            Pause,
            Insert,
            Home,
            PageUp,
            Delete,
            End,
            PageDown,
            Right,
            Left,
            Down,
            Up,
            RightArrow = Right,
            LeftArrow = Left,
            DownArrow = Down,
            UpArrow = Up,
            NumLock,
            NumpadDivide,
            NumpadMultiply,
            NumpadMinus,
            NumpadPlus,
            NumpadEqual,
            NumpadEnter,
            NumpadDecimal,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            Numpad0,
            Menu,
            LControl,
            LShift,
            LAlt,
            LSystem,
            RControl,
            RShift,
            RAlt,
            RSystem,
            LaunchApplication1,
            LaunchApplication2
        };

        using enum KeyCode;

        inline static constexpr std::size_t KeyCount = static_cast<std::size_t>(KeyCode::LaunchApplication2);

        [[nodiscard]] static constexpr std::string_view toString(KeyCode keycode) noexcept;
    };
}

namespace ikk
{
    constexpr std::string_view Keyboard::toString(KeyCode keycode) noexcept
    {
        switch (keycode)
        {
            case KeyCode::Unknown:              return "Unknown";
            case KeyCode::A:                    return "A";
            case KeyCode::B:                    return "B";
            case KeyCode::C:                    return "C";
            case KeyCode::D:                    return "D";
            case KeyCode::E:                    return "E";
            case KeyCode::F:                    return "F";
            case KeyCode::G:                    return "G";
            case KeyCode::H:                    return "H";
            case KeyCode::I:                    return "I";
            case KeyCode::J:                    return "J";
            case KeyCode::K:                    return "K";
            case KeyCode::L:                    return "L";
            case KeyCode::M:                    return "M";
            case KeyCode::N:                    return "N";
            case KeyCode::O:                    return "O";
            case KeyCode::P:                    return "P";
            case KeyCode::Q:                    return "Q";
            case KeyCode::R:                    return "R";
            case KeyCode::S:                    return "S";
            case KeyCode::T:                    return "T";
            case KeyCode::U:                    return "U";
            case KeyCode::V:                    return "V";
            case KeyCode::W:                    return "W";
            case KeyCode::X:                    return "X";
            case KeyCode::Y:                    return "Y";
            case KeyCode::Z:                    return "Z";
            case KeyCode::Num1:                 return "1";
            case KeyCode::Num2:                 return "2";
            case KeyCode::Num3:                 return "3";
            case KeyCode::Num4:                 return "4";
            case KeyCode::Num5:                 return "5";
            case KeyCode::Num6:                 return "6";
            case KeyCode::Num7:                 return "7";
            case KeyCode::Num8:                 return "8";
            case KeyCode::Num9:                 return "9";
            case KeyCode::Num0:                 return "0";
            case KeyCode::Enter:                return "Enter";
            case KeyCode::Escape:               return "Escape";
            case KeyCode::Backspace:            return "Backspace";
            case KeyCode::Tab:                  return "Tab";
            case KeyCode::Space:                return "Space";
            case KeyCode::Hyphen:               return "Hyphen (-)";
            case KeyCode::Equal:                return "Equal (=)";
            case KeyCode::LBracket:             return "Left Bracket ([)";
            case KeyCode::RBracket:             return "Right Bracket (])";
            case KeyCode::Backslash:            return "Backslash (\\)";
            case KeyCode::Semicolon:            return "Semicolon (;)";
            case KeyCode::Apostrophe:           return "Apostrophe (')";
            case KeyCode::Grave:                return "Grave (`)";
            case KeyCode::Comma:                return "Comma (,)";
            case KeyCode::Period:               return "Period (.)";
            case KeyCode::Slash:                return "Slash (/)";
            case KeyCode::F1:                   return "F1";
            case KeyCode::F2:                   return "F2";
            case KeyCode::F3:                   return "F3";
            case KeyCode::F4:                   return "F4";
            case KeyCode::F5:                   return "F5";
            case KeyCode::F6:                   return "F6";
            case KeyCode::F7:                   return "F7";
            case KeyCode::F8:                   return "F8";
            case KeyCode::F9:                   return "F9";
            case KeyCode::F10:                  return "F10";
            case KeyCode::F11:                  return "F11";
            case KeyCode::F12:                  return "F12";
            case KeyCode::F13:                  return "F13";
            case KeyCode::F14:                  return "F14";
            case KeyCode::F15:                  return "F15";
            case KeyCode::F16:                  return "F16";
            case KeyCode::F17:                  return "F17";
            case KeyCode::F18:                  return "F18";
            case KeyCode::F19:                  return "F19";
            case KeyCode::F20:                  return "F20";
            case KeyCode::F21:                  return "F21";
            case KeyCode::F22:                  return "F22";
            case KeyCode::F23:                  return "F23";
            case KeyCode::F24:                  return "F24";
            case KeyCode::CapsLock:             return "CapsLock";
            case KeyCode::PrintScreen:          return "PrintScreen";
            case KeyCode::ScrollLock:           return "ScrollLock";
            case KeyCode::Pause:                return "Pause";
            case KeyCode::Insert:               return "Insert";
            case KeyCode::Home:                 return "Home";
            case KeyCode::PageUp:               return "Page Up";
            case KeyCode::Delete:               return "Delete";
            case KeyCode::End:                  return "End";
            case KeyCode::PageDown:             return "Page Down";
            case KeyCode::Right:                return "Right (→)";
            case KeyCode::Left:                 return "Left (←)";
            case KeyCode::Down:                 return "Down (↓)";
            case KeyCode::Up:                   return "Up (↑)";
            case KeyCode::NumLock:              return "NumLock";
            case KeyCode::NumpadDivide:         return "Numpad Divide (÷)";
            case KeyCode::NumpadMultiply:       return "Numpad Multiply (×)";
            case KeyCode::NumpadMinus:          return "Numpad Minus (−)";
            case KeyCode::NumpadPlus:           return "Numpad Plus (+)";
            case KeyCode::NumpadEqual:          return "Numpad Equal (=)";
            case KeyCode::NumpadEnter:          return "Numpad Enter";
            case KeyCode::NumpadDecimal:        return "Numpad Decimal (.)";
            case KeyCode::Numpad1:              return "Numpad 1";
            case KeyCode::Numpad2:              return "Numpad 2";
            case KeyCode::Numpad3:              return "Numpad 3";
            case KeyCode::Numpad4:              return "Numpad 4";
            case KeyCode::Numpad5:              return "Numpad 5";
            case KeyCode::Numpad6:              return "Numpad 6";
            case KeyCode::Numpad7:              return "Numpad 7";
            case KeyCode::Numpad8:              return "Numpad 8";
            case KeyCode::Numpad9:              return "Numpad 9";
            case KeyCode::Numpad0:              return "Numpad 0";
            case KeyCode::Menu:                 return "Menu";
            case KeyCode::LControl:             return "Left Control";
            case KeyCode::LShift:               return "Left Shift";
            case KeyCode::LAlt:                 return "Left Alt";
            case KeyCode::LSystem:              return "Left System";
            case KeyCode::RControl:             return "Right Control";
            case KeyCode::RShift:               return "Right Shift";
            case KeyCode::RAlt:                 return "Right Alt";
            case KeyCode::RSystem:              return "Right System";
            case KeyCode::LaunchApplication1:   return "Launch Application (1)";
            case KeyCode::LaunchApplication2:   return "Launch Application (2)";
        }
        return "Unknown";
    }
}