module;

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module Application;

import std;

import JoystickManager;
import EventManager;
import SceneManager;
import NonCopyable;
import TypeUtils;
import Stopwatch;
import Renderer;
import Version;
import Window;
import Event;
import Scene;
import Time;

export namespace ikk
{
    class [[nodiscard]] Application final : NonCopyable
    {
    public:
        template<RendererType T>
        struct CreateSettings final
        {
            Window::Settings windowSettings = {};
            Version version = { .major = 0, .minor = 0, .patch = 0 };
            TypeIdentity<T> renderer = {};
        };

        template<RendererType T>
        [[nodiscard]] static std::optional<Application> create(const CreateSettings<T>& settings) noexcept;

        Application(Application&&) noexcept = default;
        Application& operator=(Application&&) noexcept = delete;

        ~Application() noexcept = default;

        void run() noexcept;
        void quit() noexcept;

        [[nodiscard]] const Version& getVersion() const noexcept;

        [[nodiscard]] const Window& getWindow() const noexcept;
        [[nodiscard]] Window& getWindow() noexcept;

        [[nodiscard]] const SceneManager& getSceneManager() const noexcept;
        [[nodiscard]] SceneManager& getSceneManager() noexcept;
    private:
        class GLFWRunTime final : NonCopyable
        {
        public:
            GLFWRunTime() noexcept;

            GLFWRunTime(GLFWRunTime&& other) noexcept;
            GLFWRunTime& operator=(GLFWRunTime&& other) noexcept;

            ~GLFWRunTime() noexcept;

            void reset() noexcept;

            [[nodiscard]] bool isValid() const noexcept;
        private:
            bool m_active = false;
        };

        GLFWRunTime m_glfwRuntime{};
        Stopwatch m_deltaTimeClock{};
        SceneManager m_sceneManager{};
        Version m_version{};
        JoystickManager m_joystickManager{};

        Window m_window;

        template<RendererType T>
        explicit Application(GLFWRunTime&& glfwRunTime, const CreateSettings<T>& settings) noexcept;

        void handleEvents(IScene& scene) noexcept;
        void update(IScene& scene) noexcept;
        void render(IScene& scene) noexcept;
    };
}

namespace ikk
{
    template<RendererType T>
    std::optional<Application> Application::create(const CreateSettings<T>& settings) noexcept
    {
        try
        {
            GLFWRunTime glfwRunTime{};

            if (glfwRunTime.isValid() == false)
                return std::nullopt;

            Application app{std::move(glfwRunTime), settings};
            return std::optional{std::move(app)};
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    void Application::run() noexcept
    {
        this->m_deltaTimeClock.start();

        while (this->m_window.shouldClose() == false)
        {
            this->m_sceneManager.update();
            this->m_window.pollEvents();

            if (const std::optional<std::reference_wrapper<IScene>> activeScene = this->m_sceneManager.getActiveScene();
                activeScene.has_value() == true)
            {
                IScene& scene = activeScene->get();
                this->handleEvents(scene);
                this->update(scene);
                this->render(scene);
            }
        }

        this->quit();
    }

    void Application::quit() noexcept
    {
        this->m_window.destroy();
        this->m_glfwRuntime.reset();
    }

    const Version& Application::getVersion() const noexcept
    {
        return this->m_version;
    }

    const Window& Application::getWindow() const noexcept
    {
        return this->m_window;
    }

    Window& Application::getWindow() noexcept
    {
        return this->m_window;
    }

    const SceneManager& Application::getSceneManager() const noexcept
    {
        return this->m_sceneManager;
    }

    SceneManager& Application::getSceneManager() noexcept
    {
        return this->m_sceneManager;
    }

    Application::GLFWRunTime::GLFWRunTime() noexcept
    {
        glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
        if (glfwInit() == GLFW_TRUE)
            this->m_active = true;
    }

    Application::GLFWRunTime::GLFWRunTime(GLFWRunTime&& other) noexcept
        : m_active(std::exchange(other.m_active, false))
    {
    }

    Application::GLFWRunTime& Application::GLFWRunTime::operator=(GLFWRunTime&& other) noexcept
    {
        if (this != &other)
        {
            this->reset();
            this->m_active = std::exchange(other.m_active, false);
        }
        return *this;
    }

    Application::GLFWRunTime::~GLFWRunTime() noexcept
    {
        this->reset();
    }

    void Application::GLFWRunTime::reset() noexcept
    {
        if (const bool value = std::exchange(this->m_active, false); value == true)
            glfwTerminate();
    }

    bool Application::GLFWRunTime::isValid() const noexcept
    {
        return this->m_active;
    }

    template<RendererType T>
    Application::Application(GLFWRunTime&& glfwRunTime, const CreateSettings<T>& settings) noexcept
        : m_glfwRuntime(std::move(glfwRunTime)), m_version(settings.version), m_window(settings.windowSettings, settings.renderer)
    {
    }

    void Application::handleEvents(IScene& scene) noexcept
    {
        EventManager& eventManager = this->m_window.getEventManager();

        std::vector<Event> joystickEvents = this->m_joystickManager.getJoystickEvents();
        eventManager.push_range(joystickEvents);

        for (const Event& event : eventManager)
        {
            this->m_joystickManager.handleEvent(event);
            scene.onEvent(event);
        }

        eventManager.clear();
    }

    void Application::update(IScene& scene) noexcept
    {
        const Time dt = this->m_deltaTimeClock.restart();
        scene.onUpdate(dt);
    }

    void Application::render(IScene& scene) noexcept
    {
        if (const std::unique_ptr<IRenderer>& renderer = this->m_window.getRenderer(); renderer != nullptr &&
            renderer->beginFrame(this->m_window.getClearColor()) == true)
        {
            scene.onRender(renderer);
            renderer->endFrame();
        }
    }
}