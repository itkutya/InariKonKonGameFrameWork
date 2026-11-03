module;

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module Event;

import NonConstructible;
import TypeUtils;
import Vector;

import InputCommon;
import Joystick;
import Keyboard;
import Mouse;

export namespace ikk
{
    class [[nodiscard]] Event final
    {
    public:
        struct Error final
        {
            std::string_view message;
        };

        struct Window final : NonConstructible
        {
            struct Closed final
            {
            };

            struct Resized final
            {
                std::uint32_t width;
                std::uint32_t height;
            };

            struct FramebufferResized final
            {
                std::uint32_t width;
                std::uint32_t height;
            };

            struct Position final
            {
                Vec2i position;
            };

            struct UnIconified final
            {
            };

            struct Iconified final
            {
            };

            struct Minimized final
            {
            };

            struct Maximized final
            {
            };

            struct FocusLost final
            {
            };

            struct FocusGained final
            {
            };

            struct CursorEntered final
            {
            };

            struct CursorLeft final
            {
            };

            struct ContentScale final
            {
                Vec2f scale;
            };

            struct Refreshed final
            {
            };

            struct FileDropped final
            {
                std::vector<std::filesystem::path> paths;
            };
        };

        //TODO:
        struct Monitor final : NonConstructible
        {
            struct Connected final
            {
                //ikk::Monitor* monitor = nullptr;
            };

            struct Disconnected final
            {
                //ikk::Monitor* monitor = nullptr;
            };
        };

        struct Input final : NonConstructible
        {
            struct Text final
            {
                char32_t unicode;
            };

            struct Keyboard final
            {
                ikk::Keyboard::ScanCode scancode;
                ikk::Keyboard::KeyCode  keycode;
                ikk::Input::Action      action;
            };

            struct Mouse final : NonConstructible
            {
                struct Button final
                {
                    ikk::Mouse::Button  button;
                    ikk::Input::Action  action;
                };

                struct Wheel final
                {
                    ikk::Mouse::Wheel   wheel;
                    float               delta;
                };

                struct Move final
                {
                    Vec2f position;
                };
            };

            struct Joystick final : NonConstructible
            {
                struct Connected final
                {
                    ikk::Joystick::ID id;
                };

                struct Disconnected final
                {
                    ikk::Joystick::ID id;
                };

                struct Button final
                {
                    ikk::Joystick::ID       id;
                    ikk::Joystick::Button   button;
                    ikk::Input::Action      action;
                };

                struct Axis final
                {
                    ikk::Joystick::ID   id;
                    ikk::Joystick::Axis axis;
                    float               delta;
                };

                struct Hat final
                {
                    ikk::Joystick::ID   id;
                    ikk::Joystick::Hat  hat;
                };
            };
        };

        using Types = std::variant<
            Error,
            Window::Closed,
            Window::Resized,
            Window::FramebufferResized,
            Window::Position,
            Window::UnIconified,
            Window::Iconified,
            Window::Minimized,
            Window::Maximized,
            Window::FocusLost,
            Window::FocusGained,
            Window::CursorEntered,
            Window::CursorLeft,
            Window::ContentScale,
            Window::Refreshed,
            Window::FileDropped,
            Monitor::Connected,
            Monitor::Disconnected,
            Input::Text,
            Input::Keyboard,
            Input::Mouse::Button,
            Input::Mouse::Wheel,
            Input::Mouse::Move,
            Input::Joystick::Connected,
            Input::Joystick::Disconnected,
            Input::Joystick::Button,
            Input::Joystick::Axis,
            Input::Joystick::Hat>;

        template<class T> requires (Is<T>::template AlternativeOf_v<Types>)
        [[nodiscard]] explicit Event(T&& type) noexcept;

        Event(const Event&) noexcept = default;
        Event(Event&&) noexcept = default;

        Event& operator=(const Event&) noexcept = default;
        Event& operator=(Event&&) noexcept = default;

        ~Event() noexcept = default;

        template<class T> requires (Is<T>::template AlternativeOf_v<Types>)
        [[nodiscard]] bool is() const noexcept;

        template<class T> requires (Is<T>::template AlternativeOf_v<Types>)
        [[nodiscard]] std::optional<T> get() const noexcept;

        template<class... Func>
        void visit(Func&&... func) const noexcept;
    private:
        Types m_data;
    };

    using WindowEvent = Event::Window;
    using InputEvent = Event::Input;
}

namespace ikk
{
    template<class T> requires (Is<T>::template AlternativeOf_v<Event::Types>)
    Event::Event(T&& type) noexcept
        : m_data(std::forward<T>(type))
    {
    }

    template<class T> requires (Is<T>::template AlternativeOf_v<Event::Types>)
    bool Event::is() const noexcept
    {
        return std::holds_alternative<T>(this->m_data);
    }

    template<class T> requires (Is<T>::template AlternativeOf_v<Event::Types>)
    std::optional<T> Event::get() const noexcept
    {
        if (this->is<T>())
            return std::optional<T>{ std::get<T>(this->m_data) };
        return std::nullopt;
    }

    struct Ignore
    {
        template<class T>
        inline constexpr void operator()(const T&) const noexcept
        {
        }
    };

    template<class... Ts>
    struct Overloaded : Ts...
    {
        using Ts::operator()...;
    };

    template<class... Ts>
    Overloaded(Ts...) -> Overloaded<Ts...>;

    template<class... Func>
    void Event::visit(Func&&... func) const noexcept
    {
        std::visit(Overloaded{ std::forward<Func>(func)..., Ignore{} }, this->m_data);
    }
}