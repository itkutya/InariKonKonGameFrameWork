module;

#include <variant>

export module InputState;

import InputCombination;
import InputCommon;

import Joystick;
import Keyboard;
import Mouse;
import Flag;

export namespace ikk
{
    class [[nodiscard]] InputState final
    {
    public:
        constexpr InputState() noexcept = default;
        template<class T>
        [[nodiscard]] explicit constexpr InputState(T data) noexcept;

        constexpr InputState(const InputState&) noexcept = default;
        constexpr InputState(InputState&&) noexcept = default;

        constexpr InputState& operator=(const InputState&) noexcept = default;
        constexpr InputState& operator=(InputState&&) noexcept = default;

        constexpr ~InputState() noexcept = default;

        template<class T>
        [[nodiscard]] constexpr operator T() const noexcept;

        [[nodiscard]] constexpr operator bool() const noexcept;

        [[nodiscard]] constexpr bool isValid() const noexcept;
    private:
        std::variant<std::monostate, Input::Action, float, Flag<Joystick::Hat::State>> m_data{};
    };
}

namespace ikk
{
    template<class T>
    constexpr InputState::InputState(T data) noexcept
        : m_data(data)
    {
    }

    template<class T>
    constexpr InputState::operator T() const noexcept
    {
        if (this->isValid() == false)
            return T{};
        return std::get<T>(this->m_data);
    }

    constexpr InputState::operator bool() const noexcept
    {
        return this->isValid();
    }

    constexpr bool InputState::isValid() const noexcept
    {
        return this->m_data.index() != 0;
    }
}