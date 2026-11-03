module;

#include <memory>
#include <string_view>
#include <utility>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module Window;

import EventCallbackFuncs;
import EventManager;
import NumericCasts;
import NonCopyable;
import TypeUtils;
import Renderer;
import Vector;
import Color;
import Log;

export namespace ikk
{
    class Window final : NonCopyable
    {
    public:
        struct Settings final
        {
            Color clearColor = Color::Miku();
            std::u8string_view title = u8"Unknown";
            Vec2u size = { 1280, 720 };
            bool vSync = true;
            bool resizable = true;
            bool decorated = true;
            //TODO: Icon, etc...
        };

        template<RendererType T>
        Window(Settings settings, TypeIdentity<T>) noexcept;

        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        ~Window() noexcept;

        void create() noexcept;

        void close() noexcept;
        void destroy() noexcept;

        void pollEvents() noexcept;

        [[nodiscard]] bool shouldClose() const noexcept;

        [[nodiscard]] const EventManager& getEventManager() const noexcept;
        [[nodiscard]] EventManager& getEventManager() noexcept;

        [[nodiscard]] const std::unique_ptr<IRenderer>& getRenderer() const noexcept;
        [[nodiscard]] std::unique_ptr<IRenderer>& getRenderer() noexcept;

        [[nodiscard]] const Window::Settings& getSettings() const noexcept;

        void setClearColor(const Color& color) noexcept;
        void setWindowTitle(std::u8string_view title) noexcept;
        void setWindowSize(Vec2u size) noexcept;
        void setVSync(bool enabled) noexcept;
        void setResizable(bool enabled) noexcept;
        void setDecorated(bool enabled) noexcept;
    private:
        GLFWwindow* m_handle = nullptr;
        EventManager m_eventManager{};
        std::unique_ptr<IRenderer> m_renderer{};
        Settings m_settings{};
    };
}

namespace ikk
{
    template<RendererType T>
    Window::Window(Settings settings, TypeIdentity<T>) noexcept
        : m_settings(std::move(settings))
    {
        this->m_renderer = std::make_unique<T>();
        if (this->m_renderer == nullptr) return;
        this->create();
    }

    Window::Window(Window&& other) noexcept
        :   m_handle(std::exchange(other.m_handle, nullptr)),
            m_eventManager(std::move(other.m_eventManager)),
            m_renderer(std::move(other.m_renderer)),
            m_settings(other.m_settings)
    {
        if (this->m_handle != nullptr)
            EventCallbackFuncs::initialize(this->m_handle);
    }

    Window& Window::operator=(Window&& other) noexcept
    {
        if (this != &other)
        {
            this->destroy();

            this->m_handle = std::exchange(other.m_handle, nullptr);
            this->m_eventManager = std::move(other.m_eventManager);
            this->m_renderer = std::move(other.m_renderer);
            this->m_settings = other.m_settings;

            if (this->m_handle != nullptr)
                EventCallbackFuncs::initialize(this->m_handle);
        }
        return *this;
    }

    Window::~Window() noexcept
    {
        this->destroy();
    }

    void Window::create() noexcept
    {
        if (this->m_handle != nullptr)
        {
            Log::Warning("Window already created.");
            return;
        }

        this->m_handle = glfwCreateWindow(I32(this->m_settings.size.x()), I32(this->m_settings.size.y()),
                                          reinterpret_cast<const char*>(this->m_settings.title.data()), nullptr, nullptr);

        if (this->m_handle == nullptr)
            return;

        if (this->m_renderer != nullptr)
            this->m_renderer->create(this->m_handle);

        if (glfwRawMouseMotionSupported() == GLFW_TRUE)
            glfwSetInputMode(this->m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        EventCallbackFuncs::initialize(this->m_handle);

        this->setVSync(this->m_settings.vSync);
        this->setResizable(this->m_settings.resizable);
        this->setDecorated(this->m_settings.decorated);
    }

    void Window::close() noexcept
    {
        if (this->m_renderer != nullptr)
        {
            this->m_renderer->destroy();
            this->m_renderer = nullptr;
        }

        if (this->m_handle != nullptr)
            glfwSetWindowShouldClose(this->m_handle, GLFW_TRUE);
    }

    void Window::destroy() noexcept
    {
        this->close();

        if (this->m_handle != nullptr)
            glfwDestroyWindow(this->m_handle);

        this->m_handle = nullptr;
    }

    void Window::pollEvents() noexcept
    {
        EventCallbackFuncs::setEventManagerForProcessing(this->m_eventManager);
        if (this->m_handle != nullptr)
            glfwPollEvents();
    }

    bool Window::shouldClose() const noexcept
    {
        if (this->m_handle == nullptr) return true;
        return BOOL(glfwWindowShouldClose(this->m_handle));
    }

    const EventManager& Window::getEventManager() const noexcept
    {
        return this->m_eventManager;
    }

    EventManager& Window::getEventManager() noexcept
    {
        return this->m_eventManager;
    }

    const std::unique_ptr<IRenderer>& Window::getRenderer() const noexcept
    {
        return this->m_renderer;
    }

    std::unique_ptr<IRenderer>& Window::getRenderer() noexcept
    {
        return this->m_renderer;
    }

    const Window::Settings& Window::getSettings() const noexcept
    {
        return this->m_settings;
    }

    void Window::setClearColor(const Color& color) noexcept
    {
        this->m_settings.clearColor = color;
    }

    void Window::setWindowTitle(std::u8string_view title) noexcept
    {
        glfwSetWindowTitle(this->m_handle, reinterpret_cast<const char*>(title.data()));
        this->m_settings.title = title;
    }

    void Window::setWindowSize(Vec2u size) noexcept
    {
        glfwSetWindowSize(this->m_handle, I32(size.x()), I32(size.y()));
        this->m_settings.size = size;
    }

    void Window::setVSync(bool enabled) noexcept
    {
        //TODO: set it using renderer...
        this->m_settings.vSync = enabled;
    }

    void Window::setResizable(bool enabled) noexcept
    {
        if (enabled == false)
            glfwSetWindowAttrib(this->m_handle, GLFW_RESIZABLE, GLFW_FALSE);
        else
            glfwSetWindowAttrib(this->m_handle, GLFW_RESIZABLE, GLFW_TRUE);
        this->m_settings.resizable = enabled;
    }

    void Window::setDecorated(bool enabled) noexcept
    {
        if (enabled == false)
            glfwSetWindowAttrib(this->m_handle, GLFW_DECORATED, GLFW_FALSE);
        else
            glfwSetWindowAttrib(this->m_handle, GLFW_DECORATED, GLFW_TRUE);
        this->m_settings.decorated = enabled;
    }
}