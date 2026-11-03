module;

#include <cstdint>

export module Version;

export namespace ikk
{
    struct Version final
    {
        std::uint8_t major = 0;
        std::uint8_t minor = 0;
        std::uint8_t patch = 0;
    };
}