module;

#include <concepts>
#include <cstdint>
#include <memory>
#include <string_view>

export module Scene;

import Renderer;
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

        inline virtual void onRender(const std::unique_ptr<IRenderer>&) noexcept
        {
        }

        [[nodiscard]] const State& getState() const noexcept;

        void activate() noexcept;
        void suspend() noexcept;
        void destroy() noexcept;
    protected:
        IScene() noexcept = default;

        IScene(const IScene&) noexcept = default;
        IScene(IScene&&) noexcept = default;

        IScene& operator=(const IScene&) noexcept = default;
        IScene& operator=(IScene&&) noexcept = default;
    private:
        State m_state{State::Uninitialized};
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
}