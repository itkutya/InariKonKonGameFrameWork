export module RenderSystem;

import MeshRenderer;
import Renderer;
import Entity;

export namespace ikk
{
    struct RenderSystem
    {
        IRenderer* renderer;

        void operator()(const Entity& entity, const MeshRenderer& drawable) const noexcept
        {
            //TODO: Check if entity has Transform, or other stuff...
            if (this->renderer != nullptr)
                this->renderer->draw(drawable.mesh, drawable.material);

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
