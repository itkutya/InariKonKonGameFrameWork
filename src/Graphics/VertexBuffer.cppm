module;

#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <concepts>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include <ranges>
#include <tuple>
#include <span>

export module VertexBuffer;

import Vertex;

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
        return { std::forward<Attributes>(attributes)... };
    }

    class [[nodiscard]] VertexBuffer final
    {
    public:
        using VertexLayout = std::vector<Vertex::AttributeInfo>;

        [[nodiscard]] constexpr VertexBuffer() noexcept = default;

        template<class... Attributes, class... VertexAttributes>
        [[nodiscard]] constexpr VertexBuffer(VertexFormat<Attributes...>, std::initializer_list<std::tuple<VertexAttributes...>> vertices) noexcept;

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
    private:
        std::vector<Vertex> m_vertices{};
        VertexLayout m_attributes{};
        std::size_t m_stride{};
    };
}

namespace ikk
{
    template<class... Attributes, class... VertexAttributes>
    constexpr VertexBuffer::VertexBuffer(VertexFormat<Attributes...>, std::initializer_list<std::tuple<VertexAttributes...>> vertices) noexcept
    {
        static_assert(sizeof...(Attributes) > 0);
        static_assert(sizeof...(VertexAttributes) == sizeof...(Attributes));
        static_assert(std::same_as<std::tuple<Attributes...>, std::tuple<VertexAttributes...>>, "Vertex format and vertex data types do not match");

        this->m_vertices.reserve(vertices.size());

        for (const std::tuple<VertexAttributes...>& vertex : vertices)
        {
            std::apply([this](const VertexAttributes&... attributes) noexcept
                {
                    this->m_vertices.emplace_back(attributes...);
                },
                vertex);
        }

        this->m_stride = Vertex::stride<VertexAttributes...>();

        const Vertex::Layout<VertexAttributes...> layout = Vertex::createLayout<VertexAttributes...>();
        this->m_attributes.assign(layout.begin(), layout.end());

        assert(std::ranges::all_of(this->m_vertices,
            [this](const Vertex& vertex) noexcept
            {
                return vertex.size() == this->m_stride;
            }));
    }

    constexpr const Vertex& VertexBuffer::operator[](std::size_t index) const noexcept
    {
        return this->m_vertices[index];
    }

    constexpr Vertex& VertexBuffer::operator[](std::size_t index) noexcept
    {
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
}