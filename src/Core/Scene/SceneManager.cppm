module;

#include <functional>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>

export module SceneManager;

import NonCopyable;
import Scene;
import Log;

export namespace ikk
{
    class SceneManager final : NonCopyable
    {
    public:
        SceneManager() noexcept = default;

        SceneManager(SceneManager&&) noexcept = default;
        SceneManager& operator=(SceneManager&&) noexcept = default;

        ~SceneManager() noexcept;

        template<SceneType T, class... Args>
        void createScene(const IScene::CreateOptions& options = { .name = "Default", .replaceActiveScene = true }, Args&&... args) noexcept;

        [[nodiscard]] std::optional<std::reference_wrapper<const IScene>> getActiveScene() const noexcept;
        [[nodiscard]] std::optional<std::reference_wrapper<IScene>> getActiveScene() noexcept;

        void update() noexcept;
    private:
        std::unordered_map<std::string, std::unique_ptr<IScene>> m_scenes{};
        IScene* m_activeScene = nullptr;

        bool m_warnedAboutMissingActiveScene = false;
    };
}

namespace ikk
{
    SceneManager::~SceneManager() noexcept
    {
        for (const std::unique_ptr<IScene>& scene : this->m_scenes | std::views::values)
        {
            if (scene->getState() == IScene::State::Active)
                scene->onExit();
            scene->onDestroy();
        }
    }

    template<SceneType T, class ... Args>
    void SceneManager::createScene(const IScene::CreateOptions& options, Args&&... args) noexcept
    {
        const std::string name = std::string{options.name};
        if (this->m_scenes.contains(name) == true) return;

        this->m_warnedAboutMissingActiveScene = false;

        auto [it, _] = this->m_scenes.emplace(name, std::make_unique<T>(std::forward<Args>(args)...));
        T* newScene = static_cast<T*>(it->second.get());
        newScene->onCreate();

        if (options.replaceActiveScene == true)
        {
            if (this->m_activeScene != nullptr)
            {
                this->m_activeScene->onExit();
                this->m_activeScene->suspend();
            }

            this->m_activeScene = newScene;

            this->m_activeScene->activate();
            this->m_activeScene->onEnter();
        }
    }

    std::optional<std::reference_wrapper<const IScene>> SceneManager::getActiveScene() const noexcept
    {
        return this->m_activeScene != nullptr ? std::optional{std::ref(*this->m_activeScene)} : std::nullopt;
    }

    std::optional<std::reference_wrapper<IScene>> SceneManager::getActiveScene() noexcept
    {
        return this->m_activeScene != nullptr ? std::optional{std::ref(*this->m_activeScene)} : std::nullopt;
    }

    void SceneManager::update() noexcept
    {
        if (this->m_activeScene != nullptr && this->m_activeScene->getState() == IScene::State::Suspended)
        {
            this->m_activeScene->onExit();
            this->m_activeScene = nullptr;
        }

        for (const std::unique_ptr<IScene>& scene : this->m_scenes | std::views::values)
        {
            if (scene->getState() != IScene::State::Deleted)
                continue;

            if (this->m_activeScene == scene.get())
            {
                scene->onExit();
                this->m_activeScene = nullptr;
            }

            scene->onDestroy();
        }

        std::erase_if(this->m_scenes,
            [](const auto& pair) noexcept
            {
                return pair.second->getState() == IScene::State::Deleted;
            });

        if (this->m_activeScene == nullptr)
        {
            auto values = this->m_scenes | std::views::values;
            const auto it = std::ranges::find_if(values,
                [](const std::unique_ptr<IScene>& scene) noexcept
                {
                    return scene->getState() == IScene::State::Active;
                });

            if (it != values.end())
            {
                this->m_activeScene = (*it).get();
                this->m_activeScene->onEnter();
            }
        }

        if (this->m_activeScene == nullptr && this->m_warnedAboutMissingActiveScene == false)
        {
            this->m_warnedAboutMissingActiveScene = true;
            Log::Warning("There are no active scenes!");
        }
    }
}