module;

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module Window;

import std;

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
            Vec2u size = { 1280u, 720u };
            bool vSync = true;
            bool resizeable = true;
            bool decorated = true;
            //TODO: Icon, etc...
        };

        template<RendererType T>
        Window(const Settings& settings, [[maybe_unused]] TypeIdentity<T>) noexcept;

        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        ~Window() noexcept;

        void create(const Settings& settings) noexcept;

        void close() noexcept;
        void destroy() noexcept;

        void pollEvents() noexcept;

        [[nodiscard]] bool shouldClose() const noexcept;

        [[nodiscard]] const EventManager& getEventManager() const noexcept;
        [[nodiscard]] EventManager& getEventManager() noexcept;

        [[nodiscard]] const std::unique_ptr<IRenderer>& getRenderer() const noexcept;
        [[nodiscard]] std::unique_ptr<IRenderer>& getRenderer() noexcept;

        [[nodiscard]] Window::Settings getSettings() const noexcept;
        [[nodiscard]] const Color& getClearColor() const noexcept;
        [[nodiscard]] std::u8string getTitle() const noexcept;
        [[nodiscard]] Vec2u getSize() const noexcept;
        [[nodiscard]] bool isVsyncEnabled() const noexcept;
        [[nodiscard]] bool isResizeable() const noexcept;
        [[nodiscard]] bool isDecorated() const noexcept;

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

        Color m_clearColor{};
    };
}

namespace ikk
{
    template<RendererType T>
    Window::Window(const Settings& settings, TypeIdentity<T>) noexcept
        : m_renderer(std::make_unique<T>()), m_clearColor(settings.clearColor)
    {
        this->create(settings);
    }

    Window::Window(Window&& other) noexcept
        :   m_handle(std::exchange(other.m_handle, nullptr)),
            m_eventManager(std::move(other.m_eventManager)),
            m_renderer(std::move(other.m_renderer)),
            m_clearColor(std::exchange(other.m_clearColor, {}))
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
            this->m_clearColor = std::exchange(other.m_clearColor, {});

            if (this->m_handle != nullptr)
                EventCallbackFuncs::initialize(this->m_handle);
        }
        return *this;
    }

    Window::~Window() noexcept
    {
        this->destroy();
    }

    void Window::create(const Settings& settings) noexcept
    {
        if (this->m_handle != nullptr)
        {
            Log::Warning("Window is already created.");
            return;
        }

        {
            const std::string safeTitle{settings.title.begin(), settings.title.end()};
            this->m_handle = glfwCreateWindow(I32(settings.size.x()), I32(settings.size.y()), safeTitle.c_str(), nullptr, nullptr);
        }

        if (this->m_handle == nullptr)
            return;

        if (this->m_renderer != nullptr)
            this->m_renderer->create(this->m_handle);

        if (glfwRawMouseMotionSupported() == GLFW_TRUE)
            glfwSetInputMode(this->m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        EventCallbackFuncs::initialize(this->m_handle);

        this->setVSync(settings.vSync);
        this->setResizable(settings.resizeable);
        this->setDecorated(settings.decorated);
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

    Window::Settings Window::getSettings() const noexcept
    {
        return Window::Settings
            {
                .clearColor = this->getClearColor(),
                .title      = this->getTitle(),
                .size       = this->getSize(),
                .vSync      = this->isVsyncEnabled(),
                .resizeable = this->isResizeable(),
                .decorated  = this->isDecorated()
            };
    }

    const Color& Window::getClearColor() const noexcept
    {
        return this->m_clearColor;
    }

    std::u8string Window::getTitle() const noexcept
    {
        const std::string_view title = glfwGetWindowTitle(this->m_handle);
        return std::u8string{title.begin(), title.end()};
    }

    Vec2u Window::getSize() const noexcept
    {
        int width = 0, height = 0;
        glfwGetWindowSize(this->m_handle, &width, &height);
        return Vec2u{static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};
    }

    bool Window::isVsyncEnabled() const noexcept
    {
        //TODO:
        return false;
    }

    bool Window::isResizeable() const noexcept
    {
        return static_cast<bool>(glfwGetWindowAttrib(this->m_handle, GLFW_RESIZABLE));
    }

    bool Window::isDecorated() const noexcept
    {
        return static_cast<bool>(glfwGetWindowAttrib(this->m_handle, GLFW_DECORATED));
    }

    void Window::setClearColor(const Color& color) noexcept
    {
        this->m_clearColor = color;
    }

    void Window::setWindowTitle(std::u8string_view title) noexcept
    {
        const std::string safeTitle{title.begin(), title.end()};
        glfwSetWindowTitle(this->m_handle, safeTitle.c_str());
    }

    void Window::setWindowSize(Vec2u size) noexcept
    {
        glfwSetWindowSize(this->m_handle, I32(size.x()), I32(size.y()));
    }

    void Window::setVSync(bool enabled) noexcept
    {
        //TODO: set it using renderer...
    }

    void Window::setResizable(bool enabled) noexcept
    {
        if (enabled == false)
            glfwSetWindowAttrib(this->m_handle, GLFW_RESIZABLE, GLFW_FALSE);
        else
            glfwSetWindowAttrib(this->m_handle, GLFW_RESIZABLE, GLFW_TRUE);
    }

    void Window::setDecorated(bool enabled) noexcept
    {
        if (enabled == false)
            glfwSetWindowAttrib(this->m_handle, GLFW_DECORATED, GLFW_FALSE);
        else
            glfwSetWindowAttrib(this->m_handle, GLFW_DECORATED, GLFW_TRUE);
    }
}