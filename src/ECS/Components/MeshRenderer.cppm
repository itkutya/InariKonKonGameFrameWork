export module MeshRenderer;

import Material;
import Mesh;

export namespace ikk
{
    struct MeshRenderer
    {
        Mesh mesh{};
        Material material{};
    };
}