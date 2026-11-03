module;

#include <algorithm>
#include <cassert>
#include <ranges>
#include <variant>
#include <vector>

export module InputCombination;

import TypeUtils;
import Joystick;
import Keyboard;
import Mouse;

export namespace ikk
{
    using AnyInput = std::variant<
        Keyboard::KeyCode,
        Keyboard::ScanCode,
        Mouse::Button,
        Mouse::Wheel,
        Joystick::Button,
        Joystick::Axis,
        Joystick::Hat::Index>;

    template<class T>
    concept InputType = Is<T>::template AlternativeOf_v<AnyInput>;

    class [[nodiscard]] InputCombination final
    {
    public:
        using Container = std::vector<AnyInput>;

        template<InputType... Ts>
        [[nodiscard]] constexpr InputCombination(Ts&&... values) noexcept;

        constexpr InputCombination(const InputCombination&) noexcept = default;
        constexpr InputCombination(InputCombination&&) noexcept = default;

        constexpr InputCombination& operator=(const InputCombination&) noexcept = default;
        constexpr InputCombination& operator=(InputCombination&&) noexcept = default;

        constexpr ~InputCombination() noexcept = default;

        [[nodiscard]] constexpr std::size_t count() const noexcept;

        [[nodiscard]] constexpr const AnyInput& operator[](std::size_t index) const noexcept;
        [[nodiscard]] constexpr AnyInput& operator[](std::size_t index) noexcept;

        template<InputType T>
        void add(const T& value) noexcept;

        void clear() noexcept;

        [[nodiscard]] constexpr Container::const_iterator begin() const noexcept;
        [[nodiscard]] constexpr Container::iterator begin() noexcept;

        [[nodiscard]] constexpr Container::const_iterator end() const noexcept;
        [[nodiscard]] constexpr Container::iterator end() noexcept;

        [[nodiscard]] constexpr auto operator==(const InputCombination& other) const noexcept;
        [[nodiscard]] constexpr auto operator!=(const InputCombination& other) const noexcept;
    private:
        Container m_values{};
    };

    template<InputType T, InputType U>
    [[nodiscard]] constexpr InputCombination operator+(T lhs, U rhs) noexcept;

    template<InputType U>
    [[nodiscard]] constexpr InputCombination operator+(const InputCombination& lhs, U rhs) noexcept;
}

namespace ikk
{
    template<InputType... Types>
    constexpr InputCombination::InputCombination(Types&&... values) noexcept
        : m_values{std::forward<Types>(values)...}
    {
    }

    constexpr std::size_t InputCombination::count() const noexcept
    {
        return this->m_values.size();
    }

    constexpr const AnyInput& InputCombination::operator[](std::size_t index) const noexcept
    {
        assert(index < this->count());
        return this->m_values[index];
    }

    constexpr AnyInput& InputCombination::operator[](std::size_t index) noexcept
    {
        assert(index < this->count());
        return this->m_values[index];
    }

    template<InputType T>
    void InputCombination::add(const T& value) noexcept
    {
        this->m_values.emplace_back(value);
    }

    void InputCombination::clear() noexcept
    {
        this->m_values.clear();
    }

    constexpr InputCombination::Container::const_iterator InputCombination::begin() const noexcept
    {
        return this->m_values.begin();
    }

    constexpr InputCombination::Container::iterator InputCombination::begin() noexcept
    {
        return this->m_values.begin();
    }

    constexpr InputCombination::Container::const_iterator InputCombination::end() const noexcept
    {
        return this->m_values.end();
    }

    constexpr InputCombination::Container::iterator InputCombination::end() noexcept
    {
        return this->m_values.end();
    }

    constexpr auto InputCombination::operator==(const InputCombination& other) const noexcept
    {
        return std::ranges::equal(this->m_values, other.m_values);
    }

    constexpr auto InputCombination::operator!=(const InputCombination& other) const noexcept
    {
        return !(*this == other);
    }

    template<InputType T, InputType U>
    constexpr InputCombination operator+(T lhs, U rhs) noexcept
    {
        return InputCombination{ lhs, rhs };
    }

    template<InputType T>
    constexpr InputCombination operator+(const InputCombination& lhs, T rhs) noexcept
    {
        InputCombination out{lhs};
        out.add(rhs);
        return out;
    }
}