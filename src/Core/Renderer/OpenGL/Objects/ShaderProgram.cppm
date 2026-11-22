module;

#include <string>
#include <utility>
#include <vector>

#include "glad/gl.h"

export module ShaderProgram;

import Shader;
import Log;

export namespace ikk
{
    class ShaderProgram
    {
    public:
        ShaderProgram() noexcept = default;
        ShaderProgram(const Shader<ShaderType::Vertex>& vertexShader, const Shader<ShaderType::Fragment>& fragmentShader) noexcept;

        ShaderProgram(const ShaderProgram&) noexcept = delete;
        ShaderProgram(ShaderProgram&& other) noexcept;

        ShaderProgram& operator=(const ShaderProgram&) noexcept = delete;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        ~ShaderProgram() noexcept;

        void bind() noexcept;

        [[nodiscard]] bool isValid() const noexcept;
    private:
        GLuint m_shaderProgram = 0;
    };
}

namespace ikk
{
    ShaderProgram::ShaderProgram(const Shader<ShaderType::Vertex>& vertexShader, const Shader<ShaderType::Fragment>& fragmentShader) noexcept
    {
        const auto compile = [](GLenum type, const char* source) noexcept
            {
                const GLuint shader = glCreateShader(type);
                glShaderSource(shader, 1, &source, nullptr);
                glCompileShader(shader);
                GLint compiled = GL_FALSE;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
                if (compiled == GL_FALSE)
                {
                    GLint length = 0;
                    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                    std::vector<GLchar> log(static_cast<std::size_t>(length));
                    glGetShaderInfoLog(shader, length, nullptr, log.data());
                    Log::Error("OpenGL shader compilation failed: {}", log.data());
                    glDeleteShader(shader);
                    return GLuint{0};
                }
                return shader;
            };

        const GLuint vert = compile(GL_VERTEX_SHADER, vertexShader.getSource().c_str());
        const GLuint frag = compile(GL_FRAGMENT_SHADER, fragmentShader.getSource().c_str());
        if (vert != 0 && frag != 0)
        {
            this->m_shaderProgram = glCreateProgram();
            glAttachShader(this->m_shaderProgram, vert);
            glAttachShader(this->m_shaderProgram, frag);
            glLinkProgram(this->m_shaderProgram);
            GLint linked = GL_FALSE;
            glGetProgramiv(this->m_shaderProgram, GL_LINK_STATUS, &linked);
            if (linked == GL_FALSE)
            {
                GLint length = 0;
                glGetProgramiv(this->m_shaderProgram, GL_INFO_LOG_LENGTH, &length);
                std::vector<GLchar> log(static_cast<std::size_t>(length));
                glGetProgramInfoLog(this->m_shaderProgram, length, nullptr, log.data());
                Log::Error("OpenGL shader link failed: {}", log.data());
                glDeleteProgram(this->m_shaderProgram);
                this->m_shaderProgram = 0;
            }
        }
        if (vert != 0) glDeleteShader(vert);
        if (frag != 0) glDeleteShader(frag);
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
        : m_shaderProgram(std::exchange(other.m_shaderProgram, 0))
    {
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
    {
        if (this != &other)
        {
            this->m_shaderProgram = std::exchange(other.m_shaderProgram, 0);
        }
        return *this;
    }

    ShaderProgram::~ShaderProgram() noexcept
    {
        if (this->m_shaderProgram != 0)
        {
            glDeleteProgram(this->m_shaderProgram);
            this->m_shaderProgram = 0;
        }
    }

    void ShaderProgram::bind() noexcept
    {
        glUseProgram(this->m_shaderProgram);
    }

    bool ShaderProgram::isValid() const noexcept
    {
        return this->m_shaderProgram != 0;
    }
}