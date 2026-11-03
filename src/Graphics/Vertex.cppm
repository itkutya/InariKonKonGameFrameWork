module;

#include <type_traits>
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cassert>
#include <vector>
#include <ranges>
#include <array>
#include <span>

export module Vertex;

import CommonVertexAttributes;

export namespace ikk
{
    class [[nodiscard]] Vertex final
    {
    public:
        struct AttributeInfo final
        {
            std::size_t offset{};
            std::size_t size{};
            std::size_t typeID{};

            [[nodiscard]] constexpr bool operator==(const AttributeInfo&) const noexcept = default;
            [[nodiscard]] constexpr bool operator!=(const AttributeInfo&) const noexcept = default;
        };

        template<class... Attributes>
        using Layout = std::array<AttributeInfo, sizeof...(Attributes)>;

        [[nodiscard]] constexpr Vertex() noexcept = default;

        template<class... Attributes> requires ((!std::same_as<std::remove_cvref_t<Attributes>, Vertex>) && ...)
        [[nodiscard]] explicit constexpr Vertex(Attributes&&... attributes) noexcept;

        constexpr Vertex(const Vertex&) noexcept = default;
        constexpr Vertex(Vertex&&) noexcept = default;

        constexpr Vertex& operator=(const Vertex&) noexcept = default;
        constexpr Vertex& operator=(Vertex&&) noexcept = default;

        constexpr ~Vertex() noexcept = default;

        void reserve(std::size_t size) noexcept;

        template<class T>
        [[nodiscard]] constexpr AttributeInfo append(T&& attribute) noexcept;

        template<class T>
        [[nodiscard]] constexpr bool contains(const AttributeInfo& info) const noexcept;

        template<class T>
        [[nodiscard]] constexpr T get(const AttributeInfo& info) const noexcept;

        template<class T>
        constexpr void set(const AttributeInfo& info, const T& value) noexcept;

        [[nodiscard]] constexpr std::size_t size() const noexcept;

        [[nodiscard]] constexpr const std::byte* data() const noexcept;
        [[nodiscard]] constexpr std::byte* data() noexcept;

        template<class... Attributes>
        [[nodiscard]] static constexpr Layout<Attributes...> createLayout() noexcept;

        template<class... Attributes>
        [[nodiscard]] static constexpr std::size_t stride() noexcept;
    private:
        std::vector<std::byte> m_data{};

        [[nodiscard]] static constexpr std::size_t alignUp(std::size_t value, std::size_t alignment) noexcept;
    };
}

namespace ikk
{
    template<class... Attributes> requires ((!std::same_as<std::remove_cvref_t<Attributes>, Vertex>) && ...)
    constexpr Vertex::Vertex(Attributes&&... attributes) noexcept
    {
        this->reserve(stride<Attributes...>());
        ((void)this->append(std::forward<Attributes>(attributes)), ...);
    }

    void Vertex::reserve(std::size_t size) noexcept
    {
        this->m_data.reserve(size);
    }

    template<class T>
    constexpr Vertex::AttributeInfo Vertex::append(T&& attribute) noexcept
    {
        using Type = std::remove_cvref_t<T>;

        static_assert(std::is_trivially_copyable_v<Type> == true);

        constexpr std::size_t alignment = alignof(Type);
        constexpr std::size_t size = sizeof(Type);

        const std::size_t offset = alignUp(this->m_data.size(), alignment);

        this->m_data.resize(offset + size);

        const auto source = std::as_bytes(std::span{std::addressof(attribute), 1});
        auto destination = std::span{this->m_data}.subspan(offset, sizeof(Type));
        std::ranges::copy(source, destination.begin());

        return AttributeInfo
            {
                .offset = offset,
                .size = size,
                .typeID = typeid(Type).hash_code()
            };
    }

    template<class T>
    constexpr bool Vertex::contains(const AttributeInfo& info) const noexcept
    {
        using Type = std::remove_cvref_t<T>;
        return  info.typeID == typeid(Type).hash_code()         &&
                info.size == sizeof(Type)                       &&
                info.offset <= this->m_data.size()              &&
                info.size <= this->m_data.size() - info.offset;
    }

    template<class T>
    constexpr T Vertex::get(const AttributeInfo& info) const noexcept
    {
        using Type = std::remove_cvref_t<T>;
        assert(this->contains<Type>(info) == true);

        Type result{};
        const auto source = std::span{this->m_data}.subspan(info.offset, info.size);
        auto destination = std::as_writable_bytes(std::span{std::addressof(result), 1});
        std::ranges::copy(source, destination.begin());
        return result;
    }

    template<class T>
    constexpr void Vertex::set(const AttributeInfo& info, const T& value) noexcept
    {
        using Type = std::remove_cvref_t<T>;

        static_assert(std::is_trivially_copyable_v<Type>);
        assert(this->contains<Type>(info));

        const auto source = std::as_bytes(std::span{std::addressof(value), 1});
        auto destination = std::span{this->m_data}.subspan(info.offset, info.size);
        std::ranges::copy(source, destination.begin());
    }

    constexpr std::size_t Vertex::size() const noexcept
    {
        return this->m_data.size();
    }

    constexpr const std::byte* Vertex::data() const noexcept
    {
        return this->m_data.data();
    }

    constexpr std::byte* Vertex::data() noexcept
    {
        return this->m_data.data();
    }

    template<class... Attributes>
    constexpr Vertex::Layout<Attributes...> Vertex::createLayout() noexcept
    {
        static_assert(sizeof...(Attributes) > 0);

        std::size_t offset = 0;
        std::size_t index = 0;
        Layout<Attributes...> result{};

        const auto addAttribute = [&]<class Type>(std::type_identity<Type>)
        {
            offset = alignUp(offset, alignof(Type));

            result[index] = AttributeInfo
            {
                .offset = offset,
                .size = sizeof(Type),
                .typeID = typeid(Type).hash_code()
            };

            offset += sizeof(Type);
            ++index;
        };

        (addAttribute(std::type_identity<std::remove_cvref_t<Attributes>>{}), ...);

        return result;
    }

    template<class... Attributes>
    constexpr std::size_t Vertex::stride() noexcept
    {
        static_assert(sizeof...(Attributes) > 0);

        std::size_t size = 0;
        ((size = alignUp(size, alignof(std::remove_cvref_t<Attributes>)) + sizeof(std::remove_cvref_t<Attributes>)), ...);
        return size;
    }

    constexpr std::size_t Vertex::alignUp(std::size_t value, std::size_t alignment) noexcept
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }
}