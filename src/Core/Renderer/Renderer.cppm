module;

#include <type_traits>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

export module Renderer;

import Color;
import Mesh;

export namespace ikk
{
    class IRenderer
    {
    public:
        IRenderer() noexcept = default;

        IRenderer(const IRenderer&) noexcept = default;
        IRenderer(IRenderer&&) noexcept = default;

        IRenderer& operator=(const IRenderer&) noexcept = default;
        IRenderer& operator=(IRenderer&&) noexcept = default;

        virtual ~IRenderer() noexcept = default;

        virtual void create(GLFWwindow* window) noexcept = 0;
        virtual void destroy() noexcept = 0;

        [[nodiscard]] virtual bool beginFrame(const Color& clearColor) noexcept = 0;
        virtual void endFrame() noexcept = 0;

        virtual void draw(const Mesh& mesh) noexcept = 0;
    };

    template<class T>
    concept RendererType = std::is_base_of_v<IRenderer, T>;
}