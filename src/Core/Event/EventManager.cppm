module;

#include <ranges>
#include <utility>
#include <vector>

export module EventManager;

import NonCopyable;
import Event;

export namespace ikk
{
    class EventManager final : NonCopyable
    {
    public:
        EventManager() noexcept = default;

        EventManager(EventManager&&) noexcept = default;
        EventManager& operator=(EventManager&&) noexcept = default;

        ~EventManager() noexcept = default;

        void push(const Event& event) noexcept;
        template<std::ranges::input_range Rng>
        void push_range(Rng&& rng) noexcept;
        void pop() noexcept;
        void clear() noexcept;

        [[nodiscard]] const Event& top() const noexcept;

        [[nodiscard]] bool empty() const noexcept;

        [[nodiscard]] std::vector<Event>::const_iterator begin() const noexcept;
        [[nodiscard]] std::vector<Event>::iterator begin() noexcept;

        [[nodiscard]] std::vector<Event>::const_iterator end() const noexcept;
        [[nodiscard]] std::vector<Event>::iterator end() noexcept;
    private:
        std::vector<Event> m_events{};
    };
}

namespace ikk
{
    void EventManager::push(const Event& event) noexcept
    {
        this->m_events.push_back(event);
    }

    template<std::ranges::input_range Rng>
    void EventManager::push_range(Rng&& rng) noexcept
    {
        this->m_events.append_range(std::forward<Rng>(rng));
    }

    void EventManager::pop() noexcept
    {
        if (this->m_events.empty() == false)
            this->m_events.pop_back();
    }

    void EventManager::clear() noexcept
    {
        this->m_events.clear();
    }

    const Event& EventManager::top() const noexcept
    {
        return this->m_events.back();
    }

    bool EventManager::empty() const noexcept
    {
        return this->m_events.empty();
    }

    std::vector<Event>::const_iterator EventManager::begin() const noexcept
    {
        return this->m_events.cbegin();
    }

    std::vector<Event>::iterator EventManager::begin() noexcept
    {
        return this->m_events.begin();
    }

    std::vector<Event>::const_iterator EventManager::end() const noexcept
    {
        return this->m_events.cend();
    }

    std::vector<Event>::iterator EventManager::end() noexcept
    {
        return this->m_events.end();
    }
}