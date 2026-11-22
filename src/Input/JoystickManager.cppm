module;

#include <algorithm>
#include <ranges>
#include <vector>

export module JoystickManager;

import PhysicalJoystick;
import InputCommon;
import Joystick;
import Event;
import Flag;

export namespace ikk
{
    class JoystickManager
    {
    public:
        JoystickManager() noexcept;

        JoystickManager(const JoystickManager&) noexcept = default;
        JoystickManager(JoystickManager&&) noexcept = default;

        JoystickManager& operator=(const JoystickManager&) noexcept = default;
        JoystickManager& operator=(JoystickManager&&) noexcept = default;

        ~JoystickManager() noexcept = default;

        std::vector<Event> getJoystickEvents() noexcept;

        void handleEvent(const Event& e) noexcept;
    private:
        std::vector<PhysicalJoystick> m_joysticks{};
    };
}

namespace ikk
{
    JoystickManager::JoystickManager() noexcept
    {
        this->m_joysticks.reserve(PhysicalJoystick::MaxJoystickCount);
        for (Joystick::ID id = 0; id <= PhysicalJoystick::MaxJoystickCount - 1; ++id)
            if (PhysicalJoystick::isConnected(id) == true)
                this->m_joysticks.emplace_back(id);
    }

    std::vector<Event> JoystickManager::getJoystickEvents() noexcept
    {
        const auto trueJoysticks =
            [](const PhysicalJoystick& physicalJoystick) noexcept
            {
                return physicalJoystick.isGamepadInputAvailable() == true;
            };

        std::vector<Event> joystickEvents{};
        for (const PhysicalJoystick& joystick : this->m_joysticks | std::views::filter(trueJoysticks))
        {
            for (std::size_t i = 1; i <= Joystick::ButtonCount; ++i)
            {
                static Input::Action previousAction = Input::Action::None;

                const auto button = static_cast<Joystick::Button>(i);
                if (const Input::Action action = joystick.getJoystickButtonState(button); previousAction != action)
                {
                    previousAction = action;
                    joystickEvents.emplace_back(InputEvent::Joystick::Button{ .id = joystick.getID(), .button = button, .action = action });
                }
            }

            for (std::size_t i = 1; i <= Joystick::AxisCount; ++i)
            {
                static float previousAxis = 0.0f;

                const auto axis = static_cast<Joystick::Axis>(i);
                if (const float delta = joystick.getJoystickAxisValue(axis); previousAxis != delta)
                {
                    previousAxis = delta;
                    joystickEvents.emplace_back(InputEvent::Joystick::Axis{ .id = joystick.getID(), .axis = axis, .delta = delta });
                }
            }

            for (Joystick::Hat::Index i = 0; i < joystick.getJoystickHatsCount(); ++i)
            {
                static Flag<Joystick::Hat::State> previousHatState = Joystick::Hat::State::Unknown;

                if (const Flag<Joystick::Hat::State> state = joystick.getJoystickHatState(i); previousHatState != state)
                {
                    previousHatState = state;
                    joystickEvents.emplace_back(InputEvent::Joystick::Hat{ .id = joystick.getID(), .hat = {.index = i, .state = state} });
                }
            }
        }
        return joystickEvents;
    }

    void JoystickManager::handleEvent(const Event& e) noexcept
    {
        e.visit(
            [this](const InputEvent::Joystick::Connected& event)
            {
                this->m_joysticks.emplace_back(event.id);
            },
            [this](const InputEvent::Joystick::Disconnected& event)
            {
                this->m_joysticks.erase(std::ranges::find_if(this->m_joysticks,
                    [&](const PhysicalJoystick& joystick) noexcept
                    {
                        return joystick.getID() == event.id;
                    }));
            });
    }
}