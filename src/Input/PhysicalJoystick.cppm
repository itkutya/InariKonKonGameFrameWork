module;

#include <cassert>
#include <cstdint>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module PhysicalJoystick;

import InputConverters;
import NumericCasts;
import InputCommon;
import Joystick;
import Flag;

export namespace ikk
{
    class [[nodiscard]] PhysicalJoystick final
    {
    public:
        inline static constexpr std::size_t MaxJoystickCount = GLFW_JOYSTICK_16 + 1;

        explicit PhysicalJoystick(Joystick::ID id) noexcept;

        PhysicalJoystick(const PhysicalJoystick&) noexcept = default;
        PhysicalJoystick(PhysicalJoystick&&) noexcept = default;

        PhysicalJoystick& operator=(const PhysicalJoystick&) noexcept = default;
        PhysicalJoystick& operator=(PhysicalJoystick&&) noexcept = default;

        ~PhysicalJoystick() noexcept = default;

        [[nodiscard]] bool operator==(const PhysicalJoystick& other) const noexcept;
        [[nodiscard]] bool operator!=(const PhysicalJoystick& other) const noexcept;

        [[nodiscard]] const Joystick::ID& getID() const noexcept;

        [[nodiscard]] static bool isConnected(Joystick::ID id) noexcept;

        [[nodiscard]] Input::Action getJoystickButtonState(Joystick::Button button) const noexcept;
        [[nodiscard]] float getJoystickAxisValue(Joystick::Axis axis) const noexcept;
        [[nodiscard]] Flag<Joystick::Hat::State> getJoystickHatState(Joystick::Hat::Index index) const noexcept;

        [[nodiscard]] std::size_t getJoystickButtonsCount() const noexcept;
        [[nodiscard]] std::size_t getJoystickAxesCount() const noexcept;
        [[nodiscard]] std::size_t getJoystickHatsCount() const noexcept;

        [[nodiscard]] std::string_view getName() const noexcept;
        [[nodiscard]] bool isGamepadInputAvailable() const noexcept;
    private:
        Joystick::ID m_id = 0;
        std::string_view m_name = {};
        bool m_isGamepadInputAvailable = false;
    };
}

namespace ikk
{
    PhysicalJoystick::PhysicalJoystick(Joystick::ID id) noexcept
        : m_id(id), m_name(glfwGetJoystickName(I32(id))), m_isGamepadInputAvailable(glfwJoystickIsGamepad(I32(id)))
    {
        assert(this->m_id <= MaxJoystickCount - 1);

        if (this->m_isGamepadInputAvailable)
            this->m_name = glfwGetGamepadName(I32(this->m_id));

        glfwSetJoystickUserPointer(I32(this->m_id), this);
    }

    bool PhysicalJoystick::operator==(const PhysicalJoystick& other) const noexcept
    {
        return this->m_id == other.m_id;
    }

    bool PhysicalJoystick::operator!=(const PhysicalJoystick& other) const noexcept
    {
        return this->m_id != other.m_id;
    }

    const Joystick::ID& PhysicalJoystick::getID() const noexcept
    {
        return this->m_id;
    }

    bool PhysicalJoystick::isConnected(Joystick::ID id) noexcept
    {
        return glfwJoystickPresent(I32(id)) == GLFW_TRUE;
    }

    Input::Action PhysicalJoystick::getJoystickButtonState(Joystick::Button button) const noexcept
    {
        const std::int32_t index = toGLFWJoystickButton(button);

        int count = 0;
        const unsigned char* buttons = glfwGetJoystickButtons(I32(this->m_id), &count);

        if (I32(index) >= count || buttons == nullptr)
            return Input::Action::None;

        if (this->m_isGamepadInputAvailable)
            if (GLFWgamepadstate state{}; glfwGetGamepadState(I32(this->m_id), &state) != GLFW_FALSE)
                return fromGLFWAction(state.buttons[index]);

        return fromGLFWAction(buttons[index]);
    }

    float PhysicalJoystick::getJoystickAxisValue(Joystick::Axis axis) const noexcept
    {
        const std::int32_t index = toGLFWAxis(axis);

        int count = 0;
        const float* axes = glfwGetJoystickAxes(I32(this->m_id), &count);

        if (I32(index) >= count || axes == nullptr)
            return 0.f;

        if (this->m_isGamepadInputAvailable)
            if (GLFWgamepadstate state{}; glfwGetGamepadState(I32(this->m_id), &state) != GLFW_FALSE)
                return state.axes[index];

        return axes[index];
    }

    Flag<Joystick::Hat::State> PhysicalJoystick::getJoystickHatState(Joystick::Hat::Index index) const noexcept
    {
        int count = 0;
        const unsigned char* hats = glfwGetJoystickHats(I32(this->m_id), &count);

        if (I32(index) >= count || hats == nullptr)
            return Joystick::Hat::State::Unknown;

        return fromGLFWHatState(hats[index]);
    }

    std::size_t PhysicalJoystick::getJoystickButtonsCount() const noexcept
    {
        int joystickButtonCount = 0;
        glfwGetJoystickButtons(I32(this->m_id), &joystickButtonCount);
        return static_cast<std::size_t>(joystickButtonCount);
    }

    std::size_t PhysicalJoystick::getJoystickAxesCount() const noexcept
    {
        int joystickAxesCount = 0;
        glfwGetJoystickAxes(I32(this->m_id), &joystickAxesCount);
        return static_cast<std::size_t>(joystickAxesCount);
    }

    std::size_t PhysicalJoystick::getJoystickHatsCount() const noexcept
    {
        int joystickHatCount = 0;
        glfwGetJoystickHats(I32(this->m_id), &joystickHatCount);
        return static_cast<std::size_t>(joystickHatCount);
    }

    std::string_view PhysicalJoystick::getName() const noexcept
    {
        return this->m_name;
    }

    bool PhysicalJoystick::isGamepadInputAvailable() const noexcept
    {
        return this->m_isGamepadInputAvailable;
    }
}