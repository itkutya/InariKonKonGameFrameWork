module;

#include "glad/gl.h"

#include "GLFW/glfw3.h"

export module OpenGL;

import std;

import ShaderProgram;
import ObjectBuffers;

import NumericCasts;
import DebugUtils;
import Renderer;
import Material;
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

        void draw(const Mesh& mesh, const Material& material) noexcept override;
    private:
        GLFWwindow* m_window{nullptr};

        std::unordered_map<const Material*, ShaderProgram> m_shaderPrograms{};
        std::unordered_map<const Mesh*, ObjectBuffers> m_objectBuffers{};
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
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
    }

    void OpenGL::create(GLFWwindow* window) noexcept
    {
        this->m_window = window;
        glfwMakeContextCurrent(this->m_window);

        if (gladLoadGL(glfwGetProcAddress) == 0)
        {
            Log::Error("Failed to initialize OpenGL context.");
            return;
        }

        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

        if constexpr (isDebugBuild() == true)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(
                [](GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) noexcept
                {
                    switch (severity)
                    {
                    case GL_DEBUG_SEVERITY_HIGH:
                        Log::Critical("GL: {}", message);
                        break;
                    case GL_DEBUG_SEVERITY_MEDIUM:
                        Log::Error("GL: {}", message);
                        break;
                    case GL_DEBUG_SEVERITY_LOW:
                        Log::Warning("GL: {}", message);
                        break;
                    case GL_DEBUG_SEVERITY_NOTIFICATION:
                        Log::Info("GL: {}", message);
                        break;
                    default:
                        Log::Info("GL: {}", message);
                        break;
                    }
                }, nullptr);
        }

        glfwSwapInterval(1);
    }

    void OpenGL::destroy() noexcept
    {
    }

    bool OpenGL::beginFrame(const Color& clearColor) noexcept
    {
        glClearColor(
            F32(clearColor.r) / F32(std::numeric_limits<std::uint8_t>::max()),
            F32(clearColor.g) / F32(std::numeric_limits<std::uint8_t>::max()),
            F32(clearColor.b) / F32(std::numeric_limits<std::uint8_t>::max()),
            F32(clearColor.a) / F32(std::numeric_limits<std::uint8_t>::max()));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return true;
    }

    void OpenGL::endFrame() noexcept
    {
        glfwSwapBuffers(this->m_window);
    }

    void OpenGL::draw(const Mesh& mesh, const Material& material) noexcept
    {
        if (mesh.vertexBuffer.vertexCount() == 0)
            return;

        auto shaderIt = this->m_shaderPrograms.find(&material);
        if (shaderIt == this->m_shaderPrograms.end())
            shaderIt = this->m_shaderPrograms.try_emplace(&material, *material.vertexShader, *material.fragmentShader).first;
        ShaderProgram& shaderProgram = shaderIt->second;
        if (shaderProgram.isValid() == false)
            return;

        auto buffersIt = this->m_objectBuffers.find(&mesh);
        if (buffersIt == this->m_objectBuffers.end())
            buffersIt = this->m_objectBuffers.try_emplace(&mesh, mesh).first;
        ObjectBuffers& objectBuffers = buffersIt->second;

        shaderProgram.bind();
        objectBuffers.bind();

        if (objectBuffers.hasIndexBuffer())
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexBuffer.size()), GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertexBuffer.vertexCount()));
    }
}