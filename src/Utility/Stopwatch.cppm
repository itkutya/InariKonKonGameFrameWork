module;

#include <chrono>

export module Stopwatch;

import Time;

export namespace ikk
{
    class [[nodiscard]] Stopwatch final
    {
    public:
        [[nodiscard]] explicit Stopwatch(bool autostart = false) noexcept;

        Stopwatch(const Stopwatch&) noexcept = default;
        Stopwatch(Stopwatch&&) noexcept = default;

        Stopwatch& operator=(const Stopwatch&) noexcept = default;
        Stopwatch& operator=(Stopwatch&&) noexcept = default;

        ~Stopwatch() noexcept = default;

        void start() noexcept;
        void stop() noexcept;

        [[nodiscard]] Time getElapsedTime() const noexcept;

        [[nodiscard]] bool isRunning() const noexcept;

        Time restart() noexcept;

        void reset() noexcept;
    private:
        std::chrono::steady_clock::time_point m_startPoint{};
        std::chrono::steady_clock::time_point m_stopPoint{};
    };
}

namespace ikk
{
    Stopwatch::Stopwatch(bool autostart) noexcept
    {
        if (autostart == true)
            this->start();
    }

    void Stopwatch::start() noexcept
    {
        if (this->isRunning() == true) return;

        const auto now = std::chrono::steady_clock::now();

        if (this->m_stopPoint != std::chrono::steady_clock::time_point{})
            this->m_startPoint += now - this->m_stopPoint;
        else
            this->m_startPoint = now;

        this->m_stopPoint = {};
    }

    void Stopwatch::stop() noexcept
    {
        if (this->isRunning() == false) return;

        this->m_stopPoint = std::chrono::steady_clock::now();
    }

    Time Stopwatch::getElapsedTime() const noexcept
    {
        if (this->isRunning() == true)
            return Time{ std::chrono::steady_clock::now() - this->m_startPoint };
        else
            return Time{ this->m_stopPoint - this->m_startPoint };
    }

    bool Stopwatch::isRunning() const noexcept
    {
        return  this->m_startPoint != std::chrono::steady_clock::time_point{} &&
                this->m_stopPoint == std::chrono::steady_clock::time_point{};
    }

    Time Stopwatch::restart() noexcept
    {
        const Time elapsed = this->getElapsedTime();

        this->m_startPoint = std::chrono::steady_clock::now();
        this->m_stopPoint = {};

        return elapsed;
    }

    void Stopwatch::reset() noexcept
    {
        this->m_startPoint = {};
        this->m_stopPoint = {};
    }
}