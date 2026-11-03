module;

#include <concepts>
#include <cstdint>
#include <string_view>

export module Scene;

import AssetManager;
import EntityManager;
import InputManager;
import Event;
import Time;

export namespace ikk
{
    class IScene
    {
    public:
        enum struct State : std::uint8_t
        {
            Uninitialized = 0,
            Deleted,
            Suspended,
            Active
        };

        struct CreateOptions
        {
            std::string_view name = "Default";
            bool replaceActiveScene = true;
        };

        virtual ~IScene() noexcept = default;

        inline virtual void onCreate() noexcept
        {
        }

        inline virtual void onDestroy() noexcept
        {
        }

        inline virtual void onEnter() noexcept
        {
        }

        inline virtual void onExit() noexcept
        {
        }

        inline virtual void onEvent(const Event&) noexcept
        {
        }

        inline virtual void onUpdate(const Time&) noexcept
        {
        }

        inline virtual void onRender() noexcept
        {
        }

        [[nodiscard]] const State& getState() const noexcept;

        void activate() noexcept;
        void suspend() noexcept;
        void destroy() noexcept;

        [[nodiscard]] const InputManager& getInputManager() const noexcept;
        [[nodiscard]] InputManager& getInputManager() noexcept;

        [[nodiscard]] const EntityManager& getEntityManager() const noexcept;
        [[nodiscard]] EntityManager& getEntityManager() noexcept;

        [[nodiscard]] const AssetManager& getAssetManager() const noexcept;
        [[nodiscard]] AssetManager& getAssetManager() noexcept;
    protected:
        IScene() noexcept = default;

        IScene(const IScene&) noexcept = default;
        IScene(IScene&&) noexcept = default;

        IScene& operator=(const IScene&) noexcept = default;
        IScene& operator=(IScene&&) noexcept = default;
    private:
        State m_state{State::Uninitialized};

        InputManager m_inputManager{};
        EntityManager m_entityManager{};
        AssetManager m_assetManager{};
    };

    template<class T>
    concept SceneType = std::is_base_of_v<IScene, T>;
}

namespace ikk
{
    const IScene::State& IScene::getState() const noexcept
    {
        return this->m_state;
    }

    void IScene::activate() noexcept
    {
        if (this->m_state == State::Deleted) return;
        this->m_state = State::Active;
    }

    void IScene::suspend() noexcept
    {
        if (this->m_state == State::Deleted) return;
        this->m_state = State::Suspended;
    }

    void IScene::destroy() noexcept
    {
        this->m_state = State::Deleted;
    }

    const InputManager& IScene::getInputManager() const noexcept
    {
        return this->m_inputManager;
    }

    InputManager& IScene::getInputManager() noexcept
    {
        return this->m_inputManager;
    }

    const EntityManager& IScene::getEntityManager() const noexcept
    {
        return this->m_entityManager;
    }

    EntityManager& IScene::getEntityManager() noexcept
    {
        return this->m_entityManager;
    }

    const AssetManager& IScene::getAssetManager() const noexcept
    {
        return this->m_assetManager;
    }

    AssetManager& IScene::getAssetManager() noexcept
    {
        return this->m_assetManager;
    }
}