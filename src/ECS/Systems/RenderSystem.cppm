module;

#include <memory>

export module RenderSystem;

import MeshRenderer;
import Renderer;
import Entity;

export namespace ikk
{
    struct RenderSystem
    {
        std::unique_ptr<IRenderer>& renderer;

        void operator()(const Entity& entity, const MeshRenderer& drawable) const noexcept
        {
            //TODO: Check if entity has Transform, or other stuff...
            if (drawable.material.shader != nullptr)
                this->renderer->draw(drawable.mesh);

            /*
             * if (renderer->checkIfExists(mesh) == false)
             *      renderer->emplace(mesh);
             *
             * if (entity.hasComponent<Transform>())
             *      renderer->draw(mesh, material, transform);
            */
        }
    };
}