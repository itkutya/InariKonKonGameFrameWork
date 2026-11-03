module;

#include <cstdint>

export module InputCommon;

import Time;

export namespace ikk
{
    struct Input
    {
        struct RepeatSettings final
        {
            Time initialDelay = 150_ms;
            Time interval = 60_ms;
        };

        enum class Action : std::uint8_t
        {
            None,
            Pressed,
            Released,
            Repeated
        };

        using enum Action;

        inline static constexpr std::size_t ActionCount = static_cast<std::size_t>(Input::Action::Repeated);
    };
}