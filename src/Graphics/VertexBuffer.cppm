export module VertexBuffer;

import std;

import Vertex;
import Assert;

export namespace ikk
{
    template<class... Attributes>
    struct VertexFormat final
    {
        using Types = std::tuple<std::remove_cvref_t<Attributes>...>;
    };

    template<class... Attributes>
    [[nodiscard]] inline constexpr VertexFormat<Attributes...>::Types vertex(Attributes&&... attributes) noexcept
    {
        return typename VertexFormat<Attributes...>::Types{ std::forward<Attributes>(attributes)... };
    }

    class [[nodiscard]] VertexBuffer final
    {
    public:
        using VertexLayout = std::vector<Vertex::AttributeInfo>;

        [[nodiscard]] constexpr VertexBuffer() noexcept = default;

        template<class... Attributes, class... Vertices>
            requires (sizeof...(Attributes) > 0) && (std::same_as<std::remove_cvref_t<Vertices>, typename VertexFormat<Attributes...>::Types> && ...)
        [[nodiscard]] constexpr VertexBuffer(std::tuple<Attributes...> first, Vertices&&... rest) noexcept;

        constexpr VertexBuffer(const VertexBuffer&) noexcept = default;
        constexpr VertexBuffer(VertexBuffer&&) noexcept = default;

        constexpr VertexBuffer& operator=(const VertexBuffer&) noexcept = default;
        constexpr VertexBuffer& operator=(VertexBuffer&&) noexcept = default;

        constexpr ~VertexBuffer() noexcept = default;

        [[nodiscard]] constexpr const Vertex& operator[](std::size_t index) const noexcept;
        [[nodiscard]] constexpr Vertex& operator[](std::size_t index) noexcept;

        [[nodiscard]] constexpr std::size_t size() const noexcept;
        [[nodiscard]] constexpr std::size_t stride() const noexcept;
        [[nodiscard]] constexpr std::size_t vertexCount() const noexcept;

        [[nodiscard]] constexpr const VertexLayout& getAttributes() const noexcept;

        [[nodiscard]] constexpr std::span<const Vertex> vertices() const noexcept;
        [[nodiscard]] constexpr std::span<Vertex> vertices() noexcept;

        [[nodiscard]] constexpr std::vector<Vertex>::const_iterator begin() const noexcept;
        [[nodiscard]] constexpr std::vector<Vertex>::iterator begin() noexcept;

        [[nodiscard]] constexpr std::vector<Vertex>::const_iterator end() const noexcept;
        [[nodiscard]] constexpr std::vector<Vertex>::iterator end() noexcept;
    private:
        std::vector<Vertex> m_vertices{};
        VertexLayout m_attributes{};
        std::size_t m_stride{};
    };
}

namespace ikk
{
    template<class... Attributes, class... Vertices>
        requires (sizeof...(Attributes) > 0) && (std::same_as<std::remove_cvref_t<Vertices>, typename VertexFormat<Attributes...>::Types> && ...)
    constexpr VertexBuffer::VertexBuffer(std::tuple<Attributes...> first, Vertices&&... rest) noexcept
        : m_stride(Vertex::calculateStride<Attributes...>())
    {
        this->m_vertices.reserve(1 + sizeof...(Vertices));

        const auto append = [this](const auto& data)
            {
                std::apply([this](const Attributes&... attributes)
                    {
                        this->m_vertices.emplace_back(attributes...);
                    }, data);
            };

        append(first);
        (append(std::forward<Vertices>(rest)), ...);

        this->m_attributes.assign_range(Vertex::createLayout<Attributes...>());

        assert(std::ranges::all_of(this->m_vertices,
            [this](const Vertex& vertex) noexcept
            {
                return vertex.size() == this->m_stride;
            }));
    }

    constexpr const Vertex& VertexBuffer::operator[](std::size_t index) const noexcept
    {
        assert(index < this->m_vertices.size());
        return this->m_vertices[index];
    }

    constexpr Vertex& VertexBuffer::operator[](std::size_t index) noexcept
    {
        assert(index < this->m_vertices.size());
        return this->m_vertices[index];
    }

    constexpr std::size_t VertexBuffer::size() const noexcept
    {
        return this->stride() * this->vertexCount();
    }

    constexpr std::size_t VertexBuffer::stride() const noexcept
    {
        return this->m_stride;
    }

    constexpr std::size_t VertexBuffer::vertexCount() const noexcept
    {
        return this->m_vertices.size();
    }

    constexpr const VertexBuffer::VertexLayout& VertexBuffer::getAttributes() const noexcept
    {
        return this->m_attributes;
    }

    constexpr std::span<const Vertex> VertexBuffer::vertices() const noexcept
    {
        return this->m_vertices;
    }

    constexpr std::span<Vertex> VertexBuffer::vertices() noexcept
    {
        return this->m_vertices;
    }

    constexpr std::vector<Vertex>::const_iterator VertexBuffer::begin() const noexcept
    {
        return this->m_vertices.begin();
    }

    constexpr std::vector<Vertex>::iterator VertexBuffer::begin() noexcept
    {
        return this->m_vertices.begin();
    }

    constexpr std::vector<Vertex>::const_iterator VertexBuffer::end() const noexcept
    {
        return this->m_vertices.end();
    }

    constexpr std::vector<Vertex>::iterator VertexBuffer::end() noexcept
    {
        return this->m_vertices.end();
    }
}