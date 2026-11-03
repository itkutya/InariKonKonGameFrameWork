module;

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module Application;

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

import RenderSystem;

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
        Application& operator=(Application&&) noexcept = default;

        ~Application() noexcept = default;

        void run() noexcept;
        void quit() noexcept;

        [[nodiscard]] const Version& getVersion() const noexcept;

        [[nodiscard]] const Window& getWindow() const noexcept;
        [[nodiscard]] Window& getWindow() noexcept;

        [[nodiscard]] const SceneManager& getSceneManager() const noexcept;
        [[nodiscard]] SceneManager& getSceneManager() noexcept;
    private:
        Stopwatch m_deltaTimeClock{};
        SceneManager m_sceneManager{};
        Version m_version{};

        Window m_window;

        template<RendererType T>
        explicit Application(const CreateSettings<T>& settings) noexcept;

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
            glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
            if (glfwInit() == GLFW_FALSE)
                return std::nullopt;

            Application app{settings};
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
        glfwTerminate();
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

    template<RendererType T>
    Application::Application(const CreateSettings<T>& settings) noexcept
        : m_version(settings.version), m_window(settings.windowSettings, settings.renderer)
    {
    }

    void Application::handleEvents(IScene& scene) noexcept
    {
        EventManager& eventManager = this->m_window.getEventManager();

        std::vector<Event> joystickEvents = scene.getInputManager().getJoystickEvents();
        eventManager.push_range(joystickEvents);

        for (const Event& event : eventManager)
        {
            scene.onEvent(event);
            scene.getInputManager().handleEvent(event);
        }

        eventManager.clear();
    }

    void Application::update(IScene& scene) noexcept
    {
        const Time dt = this->m_deltaTimeClock.restart();

        scene.getInputManager().update(dt);
        scene.getAssetManager().update();
        scene.onUpdate(dt);
    }

    void Application::render(IScene& scene) noexcept
    {
        if (std::unique_ptr<IRenderer>& renderer = this->m_window.getRenderer(); renderer != nullptr &&
            renderer->beginFrame(this->m_window.getSettings().clearColor) == true)
        {
            scene.onRender();
            scene.getEntityManager().update(RenderSystem{renderer});
            renderer->endFrame();
        }
    }
}