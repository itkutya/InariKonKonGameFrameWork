export module Shader;

import std;

import Buffer;
import Vector;
import Matrix;
import Error;

namespace ikk
{
    template<class>
    struct UniformTraits : std::false_type {};

    template<>
    struct UniformTraits<float> : std::true_type {};

    template<>
    struct UniformTraits<double> : std::true_type {};

    template<>
    struct UniformTraits<std::int32_t> : std::true_type {};

    template<>
    struct UniformTraits<std::uint32_t> : std::true_type {};

    template<>
    struct UniformTraits<Vec2f> : std::true_type {};

    template<>
    struct UniformTraits<Vec3f> : std::true_type {};

    template<>
    struct UniformTraits<Vec4f> : std::true_type {};

    template<>
    struct UniformTraits<Mat3x3f> : std::true_type {};

    template<>
    struct UniformTraits<Mat4x4f> : std::true_type {};
}

export namespace ikk
{
    //TODO:
    // - add a way to set uniform & other shader stuff
    // - make it so shader constructed form path & source are different

    template<class T>
    concept UniformDataType = UniformTraits<std::remove_cvref_t<T>>::value;

    enum struct ShaderType : std::uint8_t
    {
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

    template<ShaderType T>
    class [[nodiscard]] Shader final
    {
    public:
        [[nodiscard]] Shader() noexcept = default;
        [[nodiscard]] explicit Shader(std::filesystem::path path) noexcept;
        [[nodiscard]] explicit Shader(std::string_view src) noexcept;

        Shader(const Shader&) noexcept = default;
        Shader(Shader&&) noexcept = default;

        Shader& operator=(const Shader&) noexcept = default;
        Shader& operator=(Shader&&) noexcept = default;

        ~Shader() noexcept = default;

        [[nodiscard]] const ShaderType& getType() const noexcept;
        [[nodiscard]] const std::filesystem::path& getPath() const noexcept;
        [[nodiscard]] const std::string& getSource() const noexcept;

        //TODO: Put this into Vulkan
        // [[nodiscard]] std::expected<std::vector<std::uint32_t>, Error> convertToSPIRV() const noexcept;

        void saveToFile(const std::filesystem::path& path) const noexcept;
        void saveToFile() const noexcept;

        template<UniformDataType U>
        void setUniform(const std::string& name, const U& data) noexcept;
    private:
        ShaderType m_type = T;
        std::filesystem::path m_path = "";
        std::string m_source = {};

        std::unordered_map<std::string, std::pair<Buffer, Buffer::Descriptor>> m_uniformBuffers{};
    };

    struct BuiltInShaders
    {
        struct Colored2D
        {
            inline static const Shader<ShaderType::Vertex> vertex
                {std::string_view{
                    R"(
                    #version 460 core

                    layout(location = 0) in vec2 inPosition;
                    layout(location = 1) in vec4 inColor;

                    layout(location = 0) out vec4 fragColor;

                    void main()
                    {
                        gl_Position = vec4(inPosition, 0.0, 1.0);
                        fragColor = inColor;
                    })"
                }};
            inline static const Shader<ShaderType::Fragment> fragment
                {std::string_view{
                    R"(
                    #version 460 core

                    layout(location = 0) in vec4 fragColor;
                    layout(location = 0) out vec4 outColor;

                    void main()
                    {
                        outColor = fragColor;
                    })"
                }};
        };

        //TODO:
        struct SkyBox
        {
        };
    };
}

namespace ikk
{
    template<ShaderType T>
    Shader<T>::Shader(std::filesystem::path path) noexcept
        : m_path(std::move(path))
    {
        if (std::ifstream file{ this->m_path, std::ios::binary | std::ios::ate }; file.is_open() == true)
        {
            this->m_source.resize(static_cast<std::size_t>(file.tellg()));
            file.seekg(0);
            file.read(this->m_source.data(), static_cast<std::streamsize>(this->m_source.size()));
        }
    }

    template<ShaderType T>
    Shader<T>::Shader(std::string_view src) noexcept
        : m_source(src)
    {
    }

    template<ShaderType T>
    const ShaderType& Shader<T>::getType() const noexcept
    {
        return this->m_type;
    }

    template<ShaderType T>
    const std::filesystem::path& Shader<T>::getPath() const noexcept
    {
        return this->m_path;
    }

    template<ShaderType T>
    const std::string& Shader<T>::getSource() const noexcept
    {
        return this->m_source;
    }

    // template<ShaderType T>
    // std::expected<std::vector<std::uint32_t>, Error> Shader<T>::convertToSPIRV() const noexcept
    // {
    //     const shaderc::Compiler compiler{};
    //     shaderc::CompileOptions options{};
    //     options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
    //     options.SetOptimizationLevel(shaderc_optimization_level_performance);
    //
    //     shaderc_shader_kind kind{};
    //     switch (this->m_type)
    //     {
    //         case ShaderType::Vertex:      kind = shaderc_glsl_vertex_shader;   break;
    //         case ShaderType::Fragment:    kind = shaderc_glsl_fragment_shader; break;
    //         default: return std::unexpected(Error{"Unsupported shader type"});
    //     }
    //
    //     const shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(this->m_source, kind, this->m_path.string().c_str(), options);
    //     if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    //         return std::unexpected(Error{ module.GetErrorMessage() });
    //
    //     return std::vector<std::uint32_t>{ module.begin(), module.end() };
    // }

    template<ShaderType T>
    void Shader<T>::saveToFile(const std::filesystem::path& path) const noexcept
    {
        if (this->m_source.empty() == true) return;

        std::ofstream file{ path, std::ios::binary | std::ios::trunc };
        file.write(this->m_source.data(), static_cast<std::streamsize>(this->m_source.size()));
    }

    template<ShaderType T>
    void Shader<T>::saveToFile() const noexcept
    {
        if (this->m_path.empty() == false)
            this->saveToFile(this->m_path);
    }

    template<ShaderType T>
    template<UniformDataType U>
    void Shader<T>::setUniform(const std::string& name, const U& data) noexcept
    {
        Buffer buffer{};
        if (std::expected<Buffer::Descriptor, Error> descriptor = buffer.append(data); descriptor.has_value())
            this->m_uniformBuffers[name] = std::make_pair(buffer, descriptor.value());
    }
}