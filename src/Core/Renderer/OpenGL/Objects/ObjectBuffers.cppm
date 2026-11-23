module;

#include "glad/gl.h"

export module ObjectBuffers;

import std;

import VertexBuffer;
import TypeUtils;
import Vertex;
import Vector;
import Matrix;
import Color;
import Mesh;

export namespace ikk
{
    class ObjectBuffers
    {
    public:
        ObjectBuffers() noexcept = default;
        explicit ObjectBuffers(const Mesh& mesh) noexcept;

        ObjectBuffers(const ObjectBuffers&) noexcept = delete;
        ObjectBuffers(ObjectBuffers&& other) noexcept;

        ObjectBuffers& operator=(const ObjectBuffers&) noexcept = delete;
        ObjectBuffers& operator=(ObjectBuffers&& other) noexcept;

        ~ObjectBuffers() noexcept;

        void bind() noexcept;

        [[nodiscard]] bool hasIndexBuffer() const noexcept;
    private:
        GLuint m_vertexArray = 0;
        GLuint m_vertexBuffer = 0;
        GLuint m_indexBuffer = 0;
    };
}

namespace ikk
{
    //TODO:
    ObjectBuffers::ObjectBuffers(const Mesh& mesh) noexcept
    {
        glGenVertexArrays(1, &this->m_vertexArray);
        glGenBuffers(1, &this->m_vertexBuffer);

        glBindVertexArray(this->m_vertexArray);

        std::vector<std::byte> vertexData{};
        for (const auto& vertices : mesh.vertexBuffer.vertices())
            vertexData.append_range(vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertexData.size()),
            vertexData.data(),
            GL_STATIC_DRAW);

        enum struct AttributeType : GLenum
        {
            Float        = GL_FLOAT,
            Int          = GL_INT,
            UnsignedInt  = GL_UNSIGNED_INT,
            UnsignedByte = GL_UNSIGNED_BYTE,
        };

        struct AttributeFormat final
        {
            GLenum type{};
            GLint componentCount{};
            AttributeType attributeType{};
        };

        const auto getAttributeFormat = [] [[nodiscard]] (std::size_t hash) noexcept -> std::optional<AttributeFormat>
            {
                if (hash == getTypeHashOf<float>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 1,
                            .attributeType = AttributeType::Float
                        }};
                if (hash == getTypeHashOf<std::int32_t>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_INT,
                            .componentCount = 1,
                            .attributeType = AttributeType::Int
                        }};
                if (hash == getTypeHashOf<std::uint32_t>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_UNSIGNED_INT,
                            .componentCount = 1,
                            .attributeType = AttributeType::UnsignedInt
                        }};
                if (hash == getTypeHashOf<Vec2f>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 2,
                            .attributeType = AttributeType::Float
                        }};
                if (hash == getTypeHashOf<Vec3f>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 3,
                            .attributeType = AttributeType::Float
                        }};
                if (hash == getTypeHashOf<Vec4f>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 4,
                            .attributeType = AttributeType::Float
                        }};
                if (hash == getTypeHashOf<Mat3x3f>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 9,
                            .attributeType = AttributeType::Float
                        }};
                if (hash == getTypeHashOf<Mat4x4f>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 16,
                            .attributeType = AttributeType::Float
                        }};
                if (hash == getTypeHashOf<Color>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_UNSIGNED_BYTE,
                            .componentCount = 4,
                            .attributeType = AttributeType::UnsignedByte
                        }};
                if (hash == getTypeHashOf<Colorf>())
                    return std::optional{AttributeFormat
                        {
                            .type = GL_FLOAT,
                            .componentCount = 4,
                            .attributeType = AttributeType::Float
                        }};
                return std::nullopt;
            };

        const VertexBuffer::VertexLayout& attributes = mesh.vertexBuffer.getAttributes();
        const GLsizei stride = static_cast<GLsizei>(mesh.vertexBuffer.stride());
        GLuint index{};

        for (const Vertex::AttributeInfo& attribute : attributes)
        {
            const std::optional<AttributeFormat> format = getAttributeFormat(attribute.hash);

            if (format.has_value() == false)
                continue;

            const void* offset = reinterpret_cast<const void*>(attribute.offset);

            const bool isInteger = format->attributeType == AttributeType::Int || format->attributeType == AttributeType::UnsignedInt;

            if (isInteger == true)
                glVertexAttribIPointer(index, format->componentCount, format->type, stride, offset);
            else
                glVertexAttribPointer(index, format->componentCount, format->type,
                    format->attributeType == AttributeType::UnsignedByte ? GL_TRUE : GL_FALSE,
                    stride, offset);

            glEnableVertexAttribArray(index);
            ++index;
        }

        if (mesh.indexBuffer.empty() == false)
        {
            glGenBuffers(1, &this->m_indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(mesh.indexBuffer.size() * sizeof(std::uint32_t)),
                mesh.indexBuffer.data(), GL_STATIC_DRAW);
        }
    }

    ObjectBuffers::ObjectBuffers(ObjectBuffers&& other) noexcept
        : m_vertexArray(std::exchange(other.m_vertexArray, 0)),
          m_vertexBuffer(std::exchange(other.m_vertexBuffer, 0)),
          m_indexBuffer(std::exchange(other.m_indexBuffer, 0))
    {
    }

    ObjectBuffers& ikk::ObjectBuffers::operator=(ObjectBuffers&& other) noexcept
    {
        if (this != &other)
        {
            this->m_vertexArray = std::exchange(other.m_vertexArray, 0);
            this->m_vertexBuffer = std::exchange(other.m_vertexBuffer, 0);
            this->m_indexBuffer = std::exchange(other.m_indexBuffer, 0);
        }
        return *this;
    }

    ObjectBuffers::~ObjectBuffers() noexcept
    {
        if (this->m_indexBuffer != 0)  glDeleteBuffers(1, &this->m_indexBuffer);
        if (this->m_vertexBuffer != 0) glDeleteBuffers(1, &this->m_vertexBuffer);
        if (this->m_vertexArray != 0)  glDeleteVertexArrays(1, &this->m_vertexArray);
    }

    void ObjectBuffers::bind() noexcept
    {
        glBindVertexArray(this->m_vertexArray);
    }

    bool ObjectBuffers::hasIndexBuffer() const noexcept
    {
        return this->m_indexBuffer != 0;
    }
}