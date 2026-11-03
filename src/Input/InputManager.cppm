module;

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <ranges>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

export module InputManager;

import InputCombination;
import InputCommon;
import InputState;

import PhysicalJoystick;

import Joystick;
import Keyboard;
import Mouse;

import EventManager;
import Event;
import Flag;
import Time;
import Log;

export namespace ikk
{
    class InputManager final
    {
    public:
        InputManager() noexcept;

        InputManager(const InputManager&) noexcept = default;
        InputManager(InputManager&&) noexcept = default;

        InputManager& operator=(const InputManager&) noexcept = default;
        InputManager& operator=(InputManager&&) noexcept = default;

        ~InputManager() noexcept = default;

        void bind(const std::string& key, const InputCombination& inputList) noexcept;
        void rebind(const std::string& key, const InputCombination& inputList) noexcept;
        void unbind(const std::string& key) noexcept;

        void setRepeatConfig(const std::string& key, const Input::RepeatSettings& config) noexcept;

        //TODO: Handel other inputs like axes & joystick hats
        template<Input::Action Action, class Func> requires (std::is_invocable_v<Func>)
        void on(const std::string& key, Func&& func) noexcept;

        template<InputType T>
        [[nodiscard]] InputState getInputState(T inputType) const noexcept;

        std::vector<Event> getJoystickEvents() noexcept;

        void handleEvent(const Event& e) noexcept;
        void update(const Time& dt) noexcept;
    private:
        using Callback = std::function<void()>;
        using CallbackList = std::unordered_map<Input::Action, Callback>;

        struct RepeatTimer
        {
            bool isRepeating = false;
            bool isInitialDelay = true;
            Time deltaTime = {};
            Input::RepeatSettings config = {};
        };

        struct InputBinding final
        {
            RepeatTimer repeatTimer = {};
            InputCombination inputBindingList = {};
            CallbackList callbacks = {};
        };

        struct AnyInputHash final
        {
            std::size_t operator()(const AnyInput& input) const noexcept
            {
                return std::visit(
                    []<class T>(const T& value) noexcept -> std::size_t
                    {
                        const std::size_t valueHash = std::hash<T>{}(value);
                        const std::size_t typeHash = typeid(T).hash_code();
                        return valueHash ^ (typeHash + 0x9e3779b9U + (valueHash << 6) + (valueHash >> 2));
                    },input);
            }
        };

        std::unordered_map<AnyInput, InputState, AnyInputHash> m_inputActionMap{};
        std::unordered_map<std::string, InputBinding> m_bindings{};
        std::vector<PhysicalJoystick> m_joysticks{};
    };
}

namespace ikk
{
    InputManager::InputManager() noexcept
    {
        this->m_joysticks.reserve(PhysicalJoystick::MaxJoystickCount);
        for (Joystick::ID id = 0; id <= PhysicalJoystick::MaxJoystickCount - 1; ++id)
            if (PhysicalJoystick::isConnected(id) == true)
                this->m_joysticks.emplace_back(id);
    }

    void InputManager::bind(const std::string& key, const InputCombination& inputList) noexcept
    {
        if (const auto it = this->m_bindings.find(key); it != this->m_bindings.end())
        {
            Log::Warning("Binding already exists for key '{}'. Call rebind() instead.", key);
            return;
        }

        this->m_bindings[key].inputBindingList = inputList;
    }

    void InputManager::rebind(const std::string& key, const InputCombination& inputList) noexcept
    {
        if (const auto it = this->m_bindings.find(key); it != this->m_bindings.end())
        {
            this->unbind(key);
            this->bind(key, inputList);
        }
        else
            Log::Warning("Binding for '{}' does not exists. Call bind() first.", key);
    }

    void InputManager::unbind(const std::string& key) noexcept
    {
        if (this->m_bindings.erase(key) == 0)
            Log::Warning("Could not remove binding for key '{}', since it does not exists.", key);
    }

    void InputManager::setRepeatConfig(const std::string& key, const Input::RepeatSettings& config) noexcept
    {
        if (const auto it = this->m_bindings.find(key); it != this->m_bindings.end())
        {
            auto& [_, inputBinding] = *it;
            inputBinding.repeatTimer.config = config;
        }
        else
            Log::Warning("Cannot set repeat config for key '{}' since it could not be found.", key);
    }

    template<Input::Action Action, class Func> requires (std::is_invocable_v<Func>)
    void InputManager::on(const std::string& key, Func&& func) noexcept
    {
        if (const auto it = this->m_bindings.find(key); it != this->m_bindings.end())
        {
            auto& [_, inputBinding] = *it;
            inputBinding.callbacks[Action] = std::forward<Func>(func);
        }
        else
            Log::Warning("Cannot set callback for key '{}' since it could not be found.", key);
    }

    template<InputType T>
    InputState InputManager::getInputState(T inputType) const noexcept
    {
        if (const auto it = this->m_inputActionMap.find(inputType); it != this->m_inputActionMap.end())
            return it->second;
        else
            return InputState{};
    }

    std::vector<Event> InputManager::getJoystickEvents() noexcept
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
                const auto button = static_cast<Joystick::Button>(i);
                if (const Input::Action action = joystick.getJoystickButtonState(button); this->getInputState(button) != action)
                    joystickEvents.emplace_back(InputEvent::Joystick::Button{ .id = joystick.getID(), .button = button, .action = action });
            }

            for (std::size_t i = 1; i <= Joystick::AxisCount; ++i)
            {
                const auto axis = static_cast<Joystick::Axis>(i);
                if (const float delta = joystick.getJoystickAxisValue(axis); static_cast<float>(this->getInputState(axis)) != delta)
                    joystickEvents.emplace_back(InputEvent::Joystick::Axis{ .id = joystick.getID(), .axis = axis, .delta = delta });
            }

            for (Joystick::Hat::Index i = 0; i < joystick.getJoystickHatsCount(); ++i)
            {
                if (const Flag<Joystick::Hat::State> state = joystick.getJoystickHatState(i);
                    static_cast<Flag<Joystick::Hat::State>>(this->getInputState(i)) != state)
                    joystickEvents.emplace_back(InputEvent::Joystick::Hat{ .id = joystick.getID(), .hat = {.index = i, .state = state} });
            }
        }
        return joystickEvents;
    }

    void InputManager::handleEvent(const Event& e) noexcept
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
            },
            [this](const InputEvent::Keyboard& event)
            {
                this->m_inputActionMap[event.keycode] = InputState{event.action};
                this->m_inputActionMap[event.scancode] = InputState{event.action};
            },
            [this](const InputEvent::Mouse::Button& event)
            {
                this->m_inputActionMap[event.button] = InputState{event.action};
            },
            [this](const InputEvent::Mouse::Wheel& event)
            {
                InputState& previousValue = this->m_inputActionMap[event.wheel];
                const auto value = static_cast<float>(previousValue);
                previousValue = InputState{value + event.delta};
            },
            [this](const InputEvent::Joystick::Button& event)
            {
                this->m_inputActionMap[event.button] = InputState{event.action};
            },
            [this](const InputEvent::Joystick::Axis& event)
            {
                //TODO: Remap, because GLFW...
                // Joystick::Axis::LTrigger || Joystick::Axis::RTrigger
                // std::clamp((joystickAxisEvent.delta + 1.0f) * 0.5f, 0.0f, 1.0f);
                this->m_inputActionMap[event.axis] = InputState{event.delta};
            },
            [this](const InputEvent::Joystick::Hat& event)
            {
                this->m_inputActionMap[event.hat.index] = InputState{event.hat.state};
            });
    }

    //TODO: Multi thread it later...
    void InputManager::update(const Time& dt) noexcept
    {
        for (auto& inputBinding : this->m_bindings | std::views::values)
        {
            auto& [repeatTimer, inputCombination, callbacks] = inputBinding;

            bool isActive = true;
            for (const AnyInput& input : inputCombination)
            {
                const bool inputIsActive = std::visit(
                    [this]<class T>(const T& i) noexcept
                    {
                        const InputState inputState = this->getInputState(i);

                        if constexpr (std::is_same_v<T, Keyboard::KeyCode>  ||
                                      std::is_same_v<T, Keyboard::ScanCode> ||
                                      std::is_same_v<T, Mouse::Button>      ||
                                      std::is_same_v<T, Joystick::Button>)
                            return inputState.isValid() &&
                                (static_cast<Input::Action>(inputState) == Input::Action::Pressed ||
                                 static_cast<Input::Action>(inputState) == Input::Action::Repeated);
                        else if constexpr (std::is_same_v<T, Joystick::Axis>)
                            return inputState.isValid() && static_cast<float>(inputState) != 0.0f;
                        else
                            return inputState.isValid() &&
                                static_cast<Flag<Joystick::Hat::State>>(inputState) != Joystick::Hat::State::Center &&
                                static_cast<Flag<Joystick::Hat::State>>(inputState) != Joystick::Hat::State::Unknown;
                    }, input);

                if (inputIsActive == false)
                {
                    isActive = false;
                    break;
                }
            }

            if (isActive == false)
            {
                if (repeatTimer.isRepeating == true)
                {
                    if (const auto it = callbacks.find(Input::Action::Released); it != callbacks.end())
                        it->second();

                    repeatTimer.isRepeating = false;
                    repeatTimer.isInitialDelay = true;
                    repeatTimer.deltaTime = {};
                }
                continue;
            }

            if (repeatTimer.isRepeating == false)
            {
                if (const auto it = callbacks.find(Input::Action::Pressed); it != callbacks.end())
                    it->second();

                repeatTimer.isRepeating = true;
                repeatTimer.isInitialDelay = true;
                repeatTimer.deltaTime = {};

                continue;
            }

            repeatTimer.deltaTime += dt;
            const Time repeatThreshold = repeatTimer.isInitialDelay == true
                ? repeatTimer.config.initialDelay
                : repeatTimer.config.interval;
            if (repeatTimer.deltaTime >= repeatThreshold)
            {
                if (const auto it = callbacks.find(Input::Action::Repeated); it != callbacks.end())
                    it->second();

                repeatTimer.deltaTime = {};
                repeatTimer.isInitialDelay = false;
            }
        }

        //TODO: Button inputs do not follow the convention
        // (pressed/released triggers only 1 frame)
        // but instead they are set to GLFW values, fix it later...

        // for (auto& [input, state] : this->m_inputActionMap)
        //     std::visit(
        //         [&state]<class T>(const T&) noexcept
        //         {
        //             if constexpr (std::is_same_v<T, Joystick::Axis> ||
        //                           std::is_same_v<T, Mouse::Wheel>)
        //                 state = InputState{0.0f};
        //             else if constexpr (std::is_same_v<T, Joystick::Hat::Index>)
        //                 state = InputState{Joystick::Hat::State::Center};
        //         }, input);
    }
}