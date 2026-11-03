module;

#include <string_view>
#include <filesystem>
#include <expected>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

#include "shaderc/shaderc.hpp"

export module Shader;

import Error;

export namespace ikk
{
    class [[nodiscard]] Shader final
    {
    public:
        enum struct Type : std::uint8_t
        {
            Unknown = 0,
            Vertex,
            Fragment,
            Geometry,
            Tessellation,
            Compute,
            Primitive,
            Mesh,
            RayTracing,
            Tensor
        };

        using enum Type;

        [[nodiscard]] Shader(Type type, std::filesystem::path path) noexcept;
        [[nodiscard]] Shader(Type type, std::string_view src) noexcept;

        Shader(const Shader&) noexcept = default;
        Shader(Shader&&) noexcept = default;

        Shader& operator=(const Shader&) noexcept = default;
        Shader& operator=(Shader&&) noexcept = default;

        ~Shader() noexcept = default;

        [[nodiscard]] const Shader::Type& getType() const noexcept;
        [[nodiscard]] const std::filesystem::path& getPath() const noexcept;
        [[nodiscard]] const std::string& getSource() const noexcept;

        [[nodiscard]] std::expected<std::vector<std::uint32_t>, Error> convertToSPIRV() const noexcept;

        void saveToFile(const std::filesystem::path& path) const noexcept;
        void saveToFile() const noexcept;
    private:
        Type m_type = Type::Unknown;
        std::filesystem::path m_path = "";
        std::string m_source = {};
    };

    //TODO:
    // Add basic shaders...
    inline const Shader builtinVertexShader{Shader::Type::Vertex, std::string_view
        {R"(
            #version 450

            layout(location = 0) in vec2 inPosition;
            layout(location = 1) in vec4 inColor;

            layout(location = 0) out vec4 fragColor;

            void main()
            {
                gl_Position = vec4(inPosition, 0.0, 1.0);
                fragColor = inColor;
            })"
        }};
}

namespace ikk
{
    Shader::Shader(Type type, std::filesystem::path path) noexcept
        : m_type(type), m_path(std::move(path))
    {
        if (std::ifstream file{ this->m_path, std::ios::binary | std::ios::ate }; file.is_open() == true)
        {
            this->m_source.resize(static_cast<std::size_t>(file.tellg()));
            file.seekg(0);
            file.read(this->m_source.data(), static_cast<std::streamsize>(this->m_source.size()));
        }
    }
    
    Shader::Shader(Type type, std::string_view src) noexcept
        : m_type(type), m_source(src)
    {
    }

    const Shader::Type& Shader::getType() const noexcept
    {
        return this->m_type;
    }

    const std::filesystem::path& Shader::getPath() const noexcept
    {
        return this->m_path;
    }

    const std::string& Shader::getSource() const noexcept
    {
        return this->m_source;
    }

    std::expected<std::vector<std::uint32_t>, Error> Shader::convertToSPIRV() const noexcept
    {
        const shaderc::Compiler compiler{};
        shaderc::CompileOptions options{};
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        shaderc_shader_kind kind = {};
        switch (this->m_type)
        {
            case Shader::Type::Vertex:      kind = shaderc_glsl_vertex_shader;   break;
            case Shader::Type::Fragment:    kind = shaderc_glsl_fragment_shader; break;
        default: return std::unexpected(Error{"Unsupported shader type"});
        }

        const shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(this->m_source, kind, this->m_path.string().c_str(), options);
        if (module.GetCompilationStatus() != shaderc_compilation_status_success)
            return std::unexpected(Error{ module.GetErrorMessage() });

        return std::vector<std::uint32_t>{ module.begin(), module.end() };
    }

    void Shader::saveToFile(const std::filesystem::path& path) const noexcept
    {
        if (this->m_source.empty() == true) return;

        std::ofstream file{ path, std::ios::binary | std::ios::trunc };
        file.write(this->m_source.data(), static_cast<std::streamsize>(this->m_source.size()));
    }

    void Shader::saveToFile() const noexcept
    {
        if (this->m_path.empty() == false)
            this->saveToFile(this->m_path);
    }
}