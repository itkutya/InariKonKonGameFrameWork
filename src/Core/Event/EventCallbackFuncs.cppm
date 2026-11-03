module;

#include <cassert>
#include <filesystem>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module EventCallbackFuncs;

import NonConstructible;
import NumericCasts;
import EventManager;
import DebugUtils;
import Vector;
import Event;
import Log;

import InputConverters;
import InputCommon;
import Keyboard;
import Mouse;

export namespace ikk
{
    class EventCallbackFuncs final : public NonConstructible
    {
    public:
        static void initialize(GLFWwindow* window) noexcept;
        static void setEventManagerForProcessing(EventManager& eventManager) noexcept;
    private:
        static void errorCallback(int code, const char* description) noexcept;
        static void windowClosedCallback(GLFWwindow* window) noexcept;
        static void windowResizeCallback(GLFWwindow* window, int width, int height) noexcept;
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) noexcept;
        static void windowContentScaleCallback(GLFWwindow* window, float xscale, float yscale) noexcept;
        static void windowPositionCallback(GLFWwindow* window, int xpos, int ypos) noexcept;
        static void windowIconifyCallback(GLFWwindow* window, int iconified) noexcept;
        static void windowMaximizeCallback(GLFWwindow* window, int maximized) noexcept;
        static void windowFocusCallback(GLFWwindow* window, int focused) noexcept;
        static void windowRefreshCallback(GLFWwindow* window) noexcept;
        static void monitorCallback(GLFWmonitor* monitor, int event) noexcept;
        static void cursorEnterCallback(GLFWwindow* window, int entered) noexcept;
        static void characterCallback(GLFWwindow* window, unsigned int codepoint) noexcept;
        static void keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept;
        static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) noexcept;
        static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) noexcept;
        static void joystickCallback(int jid, int event) noexcept;
        static void dropCallback(GLFWwindow* window, int count, const char** paths) noexcept;

        inline static EventManager* s_eventManager = nullptr;
    };
}

namespace ikk
{
    void EventCallbackFuncs::initialize(GLFWwindow* window) noexcept
    {
        assert(window != nullptr);

        if constexpr (isDebugBuild() == true) glfwSetErrorCallback(errorCallback);
        glfwSetWindowCloseCallback(window, windowClosedCallback);
        glfwSetWindowSizeCallback(window, windowResizeCallback);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetWindowPosCallback(window, windowPositionCallback);
        glfwSetWindowIconifyCallback(window, windowIconifyCallback);
        glfwSetWindowMaximizeCallback(window, windowMaximizeCallback);
        glfwSetWindowFocusCallback(window, windowFocusCallback);
        glfwSetCursorEnterCallback(window, cursorEnterCallback);
        glfwSetCharCallback(window, characterCallback);
        glfwSetKeyCallback(window, keyInputCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetScrollCallback(window, mouseScrollCallback);
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetWindowRefreshCallback(window, windowRefreshCallback);
        glfwSetWindowContentScaleCallback(window, windowContentScaleCallback);
        glfwSetDropCallback(window, dropCallback);
        glfwSetMonitorCallback(monitorCallback);
        glfwSetJoystickCallback(joystickCallback);
    }

    void EventCallbackFuncs::setEventManagerForProcessing(EventManager& eventManager) noexcept
    {
        s_eventManager = &eventManager;
    }

    void EventCallbackFuncs::errorCallback(int code, const char* description) noexcept
    {
        assert(s_eventManager != nullptr);

        Log::Error("code: {}, description: {}", code, description);
        s_eventManager->push(Event{Event::Error{ .message = description }});
    }

    void EventCallbackFuncs::windowClosedCallback([[maybe_unused]] GLFWwindow*) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{WindowEvent::Closed{}});
    }

    void EventCallbackFuncs::windowResizeCallback([[maybe_unused]] GLFWwindow*, int width, int height) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{WindowEvent::Resized{ .width = U32(width), .height = U32(height) }});
    }

    void EventCallbackFuncs::framebufferResizeCallback([[maybe_unused]] GLFWwindow*, int width, int height) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{WindowEvent::FramebufferResized{ .width = U32(width), .height = U32(height) }});
    }

    void EventCallbackFuncs::windowContentScaleCallback([[maybe_unused]] GLFWwindow*, float xscale, float yscale) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{WindowEvent::ContentScale{ .scale = Vec2f{ xscale, yscale } }});
    }

    void EventCallbackFuncs::windowPositionCallback([[maybe_unused]] GLFWwindow*, int xpos, int ypos) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{WindowEvent::Position{ .position = Vec2i{ xpos, ypos } }});
    }

    void EventCallbackFuncs::windowIconifyCallback([[maybe_unused]] GLFWwindow*, int iconified) noexcept
    {
        assert(s_eventManager != nullptr);

        if (iconified == GLFW_TRUE) s_eventManager->push(Event{WindowEvent::Iconified{}});
        else if (iconified == GLFW_FALSE) s_eventManager->push(Event{WindowEvent::UnIconified{}});
    }

    void EventCallbackFuncs::windowMaximizeCallback([[maybe_unused]] GLFWwindow*, int maximized) noexcept
    {
        assert(s_eventManager != nullptr);

        if (maximized == GLFW_TRUE) s_eventManager->push(Event{WindowEvent::Maximized{}});
        else if (maximized == GLFW_FALSE) s_eventManager->push(Event{WindowEvent::Minimized{}});
    }

    void EventCallbackFuncs::windowFocusCallback([[maybe_unused]] GLFWwindow*, int focused) noexcept
    {
        assert(s_eventManager != nullptr);

        if (focused == GLFW_TRUE) s_eventManager->push(Event{WindowEvent::FocusGained{}});
        else if (focused == GLFW_FALSE) s_eventManager->push(Event{WindowEvent::FocusLost{}});
    }

    void EventCallbackFuncs::windowRefreshCallback([[maybe_unused]] GLFWwindow*) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{WindowEvent::Refreshed{}});
    }

    //TODO:
    void EventCallbackFuncs::monitorCallback(GLFWmonitor* monitor, int event) noexcept
    {
        assert(s_eventManager != nullptr);

        if (event == GLFW_CONNECTED) s_eventManager->push(Event{Event::Monitor::Connected{/*.monitor = reinterpret_cast<Monitor*>(monitor) */}});
        else if (event == GLFW_DISCONNECTED) s_eventManager->push(Event{Event::Monitor::Disconnected{/*.monitor = reinterpret_cast<Monitor*>(monitor)*/}});
    }

    void EventCallbackFuncs::cursorEnterCallback([[maybe_unused]] GLFWwindow*, int entered) noexcept
    {
        assert(s_eventManager != nullptr);

        if (entered == GLFW_TRUE) s_eventManager->push(Event{WindowEvent::CursorEntered{}});
        else if (entered == GLFW_FALSE) s_eventManager->push(Event{WindowEvent::CursorLeft{}});
    }

    void EventCallbackFuncs::characterCallback([[maybe_unused]] GLFWwindow*, unsigned int codepoint) noexcept
    {
        assert(s_eventManager != nullptr);

        s_eventManager->push(Event{InputEvent::Text{ .unicode = static_cast<char32_t>(codepoint) }});
    }

    void EventCallbackFuncs::keyInputCallback([[maybe_unused]] GLFWwindow*, int key, int scancode, int action, [[maybe_unused]] int mods) noexcept
    {
        assert(s_eventManager != nullptr);

        const Keyboard::KeyCode keycode = fromGLFWKeyCode(key);
        const Input::Action state = fromGLFWAction(action);
        s_eventManager->push(Event{InputEvent::Keyboard{ .scancode = scancode, .keycode = keycode, .action = state }});
    }

    void EventCallbackFuncs::mouseButtonCallback([[maybe_unused]] GLFWwindow*, int button, int action, [[maybe_unused]] int mods) noexcept
    {
        assert(s_eventManager != nullptr);

        const Mouse::Button mButton = fromGLFWMouseButton(button);
        const Input::Action state = fromGLFWAction(action);
        s_eventManager->push(Event{InputEvent::Mouse::Button{ .button = mButton, .action = state }});
    }

    void EventCallbackFuncs::mouseScrollCallback([[maybe_unused]] GLFWwindow*, double xoffset, double yoffset) noexcept
    {
        assert(s_eventManager != nullptr);

        if (xoffset != 0.0)
            s_eventManager->push(Event{InputEvent::Mouse::Wheel{ .wheel = Mouse::Wheel::Horizontal, .delta = F32(xoffset) }});

        if (yoffset != 0.0)
            s_eventManager->push(Event{InputEvent::Mouse::Wheel{ .wheel = Mouse::Wheel::Vertical, .delta = F32(yoffset) }});
    }

    void EventCallbackFuncs::cursorPositionCallback([[maybe_unused]] GLFWwindow*, double xpos, double ypos) noexcept
    {
        s_eventManager->push(Event{InputEvent::Mouse::Move{ .position = { F32(xpos), F32(ypos) } }});
    }

    void EventCallbackFuncs::joystickCallback(int jid, int event) noexcept
    {
        assert(s_eventManager != nullptr);

        if (event == GLFW_CONNECTED)
            s_eventManager->push(Event{InputEvent::Joystick::Connected{ .id = U32(jid) }});
        else if (event == GLFW_DISCONNECTED)
            s_eventManager->push(Event{InputEvent::Joystick::Disconnected{ .id = U32(jid) }});
    }

    void EventCallbackFuncs::dropCallback([[maybe_unused]] GLFWwindow*, int count, const char** paths) noexcept
    {
        assert(s_eventManager != nullptr);

        std::vector<std::filesystem::path> files{};
        files.reserve(static_cast<std::size_t>(count));
        for (std::size_t i = 0; i < static_cast<std::size_t>(count); ++i)
            files.emplace_back(paths[i]);

        s_eventManager->push(Event{WindowEvent::FileDropped{ .paths = std::move(files) }});
    }
}