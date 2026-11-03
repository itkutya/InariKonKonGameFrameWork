module;

#include <cstdint>
#include <vector>

export module Mesh;

import VertexBuffer;

export namespace ikk
{
    using IndexBuffer = std::vector<std::uint32_t>;

    //TODO: load mesh from file
    struct Mesh
    {
        VertexBuffer vertexBuffer{};
        IndexBuffer indexBuffer{};
    };

    //TODO:
    // ResourceLoader impl...
}