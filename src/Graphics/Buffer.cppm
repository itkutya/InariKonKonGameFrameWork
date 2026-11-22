export module Buffer;

import std;

import TypeUtils;
import Assert;
import Error;

export namespace ikk
{
    template<class T>
    concept BufferCompatible = std::is_trivially_copyable_v<std::remove_cvref_t<T>>;

    class [[nodiscard]] Buffer final
    {
    public:
        struct [[nodiscard]] Descriptor final
        {
            std::size_t offset{};
            std::size_t size{};
            std::size_t hash{};
        };

        [[nodiscard]] constexpr Buffer() noexcept = default;

        [[nodiscard]] explicit constexpr Buffer(std::size_t size) noexcept;

        constexpr Buffer(const Buffer&) noexcept = default;
        constexpr Buffer(Buffer&&) noexcept = default;

        constexpr Buffer& operator=(const Buffer&) noexcept = default;
        constexpr Buffer& operator=(Buffer&&) noexcept = default;

        constexpr ~Buffer() noexcept = default;

        [[nodiscard]] constexpr std::size_t size() const noexcept;
        [[nodiscard]] constexpr bool empty() const noexcept;

        constexpr void reserve(std::size_t size) noexcept;
        constexpr void resize(std::size_t size) noexcept;
        constexpr void clear() noexcept;

        [[nodiscard]] constexpr std::span<const std::byte> data() const noexcept;
        [[nodiscard]] constexpr std::span<std::byte> data() noexcept;

        template<BufferCompatible T>
        [[nodiscard]] constexpr bool contains(const Buffer::Descriptor& descriptor) const noexcept;

        template<BufferCompatible T>
        [[nodiscard]] constexpr std::expected<Buffer::Descriptor, Error> append(const T& data, std::size_t alignment = 1) noexcept;

        template<BufferCompatible T>
        [[nodiscard]] constexpr std::expected<T, Error> get(const Buffer::Descriptor& descriptor) const noexcept;

        template<BufferCompatible T>
        constexpr void set(const Buffer::Descriptor& descriptor, const T& value) noexcept;
    private:
        std::vector<std::byte> m_data{};
    };
}

namespace ikk
{
    constexpr Buffer::Buffer(std::size_t size) noexcept
    {
        this->m_data.resize(size);
    }

    constexpr std::size_t Buffer::size() const noexcept
    {
        return this->m_data.size();
    }

    constexpr bool Buffer::empty() const noexcept
    {
        return this->m_data.empty();
    }

    constexpr void Buffer::reserve(std::size_t size) noexcept
    {
        this->m_data.reserve(size);
    }

    constexpr void Buffer::resize(std::size_t size) noexcept
    {
        this->m_data.resize(size);
    }

    constexpr void Buffer::clear() noexcept
    {
        this->m_data.clear();
    }

    constexpr std::span<const std::byte> Buffer::data() const noexcept
    {
        return std::span<const std::byte>{this->m_data};
    }

    constexpr std::span<std::byte> Buffer::data() noexcept
    {
        return std::span<std::byte>{this->m_data};
    }

    template<BufferCompatible T>
    constexpr bool Buffer::contains(const Buffer::Descriptor& descriptor) const noexcept
    {
        return  descriptor.hash == getTypeID<T>()                       &&
                descriptor.size >= sizeof(T)                            &&
                descriptor.offset <= this->m_data.size()                &&
                descriptor.size <= this->m_data.size() - descriptor.offset;
    }

    template<BufferCompatible T>
    constexpr std::expected<Buffer::Descriptor, Error> Buffer::append(const T& data, std::size_t alignment) noexcept
    {
        try
        {
            if (std::has_single_bit(alignment) == false)
                return std::unexpected{Error{"Align not supported"}};

            const auto alignUp = [](std::size_t value, std::size_t align) noexcept -> std::size_t
                {
                    return (value + align - 1) & ~(align - 1);
                };

            alignment = std::max(alignment, alignof(T));

            const std::size_t max = this->m_data.max_size();
            if (alignment > max / 2)
                return std::unexpected{Error{"Alignment too large"}};

            const std::size_t offset = alignUp(this->m_data.size(), alignment);
            const std::size_t size = alignUp(sizeof(T), alignment);

            if (offset > max - size)
                return std::unexpected{Error{"Buffer too large"}};

            this->m_data.resize(offset + size);

            std::ranges::copy(
                std::as_bytes(std::span{std::addressof(data), 1}),
                std::span{this->m_data}.subspan(offset).begin());

            return std::expected<Buffer::Descriptor, Error>{Buffer::Descriptor
                {
                    .offset = offset,
                    .size = size,
                    .hash = getTypeID<T>()
                }};
        }
        catch (...)
        {
            return std::unexpected{Error{"Unknown exception"}};
        }
    }

    template<BufferCompatible T>
    constexpr std::expected<T, Error> Buffer::get(const Buffer::Descriptor& descriptor) const noexcept
    {
        if (this->contains<T>(descriptor) == false)
            return std::unexpected{Error{"Buffer does not contain the searched value!"}};

        std::array<std::byte, sizeof(T)> bytes{};
        std::ranges::copy(this->data().subspan(descriptor.offset, sizeof(T)), bytes.begin());

        return std::expected<T, Error>{std::bit_cast<T>(bytes)};
    }

    template<BufferCompatible T>
    constexpr void Buffer::set(const Buffer::Descriptor& descriptor, const T& value) noexcept
    {
        assert(this->contains<T>(descriptor) == true);

        const auto source = std::as_bytes(std::span{std::addressof(value), 1});
        auto destination = std::span{this->m_data}.subspan(descriptor.offset, descriptor.size);
        std::ranges::copy(source, destination.begin());
    }
}