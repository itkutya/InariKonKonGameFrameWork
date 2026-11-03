module;

#include <cstdint>

export module Dimension;

export namespace ikk
{
    enum struct Dimension : std::uint8_t
    {
        _2D = 2, _3D = 3
    };
}