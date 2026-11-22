export module Vertex;

import std;

import CommonVertexAttributes;
import Buffer;
import Error;

export namespace ikk
{
    class [[nodiscard]] Vertex final
    {
    public:
        using AttributeInfo = Buffer::Descriptor;

        template<BufferCompatible... Attributes>
        using Layout = std::array<Vertex::AttributeInfo, sizeof...(Attributes)>;

        [[nodiscard]] constexpr Vertex() noexcept = default;

        template<BufferCompatible... Attributes> requires ((!std::same_as<std::remove_cvref_t<Attributes>, Vertex>) && ...)
        [[nodiscard]] explicit constexpr Vertex(Attributes&&... attributes) noexcept;

        constexpr Vertex(const Vertex&) noexcept = default;
        constexpr Vertex(Vertex&&) noexcept = default;

        constexpr Vertex& operator=(const Vertex&) noexcept = default;
        constexpr Vertex& operator=(Vertex&&) noexcept = default;

        constexpr ~Vertex() noexcept = default;

        [[nodiscard]] constexpr std::size_t size() const noexcept;

        [[nodiscard]] constexpr std::span<const std::byte> data() const noexcept;
        [[nodiscard]] constexpr std::span<std::byte> data() noexcept;

        void reserve(std::size_t size) noexcept;

        template<BufferCompatible T>
        [[nodiscard]] constexpr bool contains(const Vertex::AttributeInfo& info) const noexcept;

        template<BufferCompatible T>
        [[nodiscard]] constexpr std::expected<Vertex::AttributeInfo, Error> append(T&& attribute) noexcept;

        template<BufferCompatible T>
        [[nodiscard]] constexpr std::expected<T, Error> get(const Vertex::AttributeInfo& info) const noexcept;

        template<BufferCompatible T>
        constexpr void set(const Vertex::AttributeInfo& info, const T& value) noexcept;

        template<BufferCompatible... Attributes>
        [[nodiscard]] static constexpr Layout<Attributes...> createLayout() noexcept;

        template<BufferCompatible... Attributes>
        [[nodiscard]] static constexpr std::size_t calculateStride() noexcept;
    private:
        Buffer m_buffer{};
    };
}

namespace ikk
{
    template<BufferCompatible... Attributes> requires ((!std::same_as<std::remove_cvref_t<Attributes>, Vertex>) && ...)
    constexpr Vertex::Vertex(Attributes&&... attributes) noexcept
    {
        this->m_buffer.reserve(calculateStride<Attributes...>());
        ((void)this->append(std::forward<Attributes>(attributes)), ...);
    }

    constexpr std::size_t Vertex::size() const noexcept
    {
        return this->m_buffer.size();
    }

    constexpr std::span<const std::byte> Vertex::data() const noexcept
    {
        return this->m_buffer.data();
    }

    constexpr std::span<std::byte> Vertex::data() noexcept
    {
        return this->m_buffer.data();
    }

    void Vertex::reserve(std::size_t size) noexcept
    {
        this->m_buffer.reserve(size);
    }

    template<BufferCompatible T>
    constexpr bool Vertex::contains(const AttributeInfo& info) const noexcept
    {
        return this->m_buffer.contains<T>(info);
    }

    template<BufferCompatible T>
    constexpr std::expected<Vertex::AttributeInfo, Error> Vertex::append(T&& attribute) noexcept
    {
        return this->m_buffer.append(attribute);
    }

    template<BufferCompatible T>
    constexpr std::expected<T, Error> Vertex::get(const Vertex::AttributeInfo& info) const noexcept
    {
        return this->m_buffer.get<T>(info);
    }

    template<BufferCompatible T>
    constexpr void Vertex::set(const AttributeInfo& info, const T& value) noexcept
    {
        this->m_buffer.set(info, value);
    }

    template<BufferCompatible... Attributes>
    constexpr Vertex::Layout<Attributes...> Vertex::createLayout() noexcept
    {
        static_assert(sizeof...(Attributes) > 0);

        std::size_t offset = 0;
        std::size_t index = 0;
        Vertex::Layout<Attributes...> result{};

        const auto addAttribute = [&]<class T>(std::type_identity<T>)
        {
            const auto alignUp = [](std::size_t value, std::size_t align) noexcept -> std::size_t
                {
                    return (value + align - 1) & ~(align - 1);
                };

            offset = alignUp(offset, alignof(T));

            result[index] = Vertex::AttributeInfo
                    {
                        .offset = offset,
                        .size = sizeof(T),
                        .hash = typeid(T).hash_code()
                    };

            offset += sizeof(T);
            ++index;
        };

        (addAttribute(std::type_identity<std::remove_cvref_t<Attributes>>{}), ...);

        return result;
    }

    template<BufferCompatible... Attributes>
    constexpr std::size_t Vertex::calculateStride() noexcept
    {
        static_assert(sizeof...(Attributes) > 0);

        const auto alignUp = [](std::size_t value, std::size_t align) noexcept -> std::size_t
            {
                return (value + align - 1) & ~(align - 1);
            };

        std::size_t size = 0;
        ((size = alignUp(size, alignof(std::remove_cvref_t<Attributes>)) + sizeof(std::remove_cvref_t<Attributes>)), ...);
        return size;
    }
}
