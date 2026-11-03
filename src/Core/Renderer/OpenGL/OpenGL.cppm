module;

#include <cstdint>
#include <limits>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

export module OpenGL;

import NumericCasts;
import Renderer;
import Color;
import Mesh;
import Log;

export namespace ikk
{
    class OpenGL final : public IRenderer
    {
    public:
        OpenGL() noexcept;

        OpenGL(const OpenGL&) noexcept = default;
        OpenGL(OpenGL&&) noexcept = default;

        OpenGL& operator=(const OpenGL&) noexcept = default;
        OpenGL& operator=(OpenGL&&) noexcept = default;

        ~OpenGL() noexcept override = default;

        void create(GLFWwindow* window) noexcept override;
        void destroy() noexcept override;

        [[nodiscard]] bool beginFrame(const Color& clearColor) noexcept override;
        void endFrame() noexcept override;

        void draw(const Mesh& mesh) noexcept override;
    private:
        GLFWwindow* m_window{nullptr};
    };
}

namespace ikk
{
    OpenGL::OpenGL() noexcept
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void OpenGL::create(GLFWwindow* window) noexcept
    {
        this->m_window = window;
        glfwMakeContextCurrent(this->m_window);

        if (gladLoadGL(glfwGetProcAddress) == 0)
            Log::Error("Failed to initialize OpenGL context.");

        glfwSwapInterval(1);
    }

    void OpenGL::destroy() noexcept
    {
    }

    bool OpenGL::beginFrame(const Color& clearColor) noexcept
    {
        glClearColor(
            clearColor.r / F32(std::numeric_limits<std::uint8_t>::max()),
            clearColor.g / F32(std::numeric_limits<std::uint8_t>::max()),
            clearColor.b / F32(std::numeric_limits<std::uint8_t>::max()),
            clearColor.a / F32(std::numeric_limits<std::uint8_t>::max()));
        glClear(GL_COLOR_BUFFER_BIT);
        return true;
    }

    void OpenGL::endFrame() noexcept
    {
        glfwSwapBuffers(this->m_window);
    }

    void OpenGL::draw(const Mesh& mesh) noexcept
    {
    }
}