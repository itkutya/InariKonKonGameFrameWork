module;

#include <type_traits>

export module Flag;

export namespace ikk
{
    template<class T>
    class [[nodiscard]] Flag final
    {
        using Type = std::underlying_type_t<T>;
    public:
        static_assert(std::is_enum_v<T>, "T must be an enum");

        constexpr Flag() noexcept = default;
        [[nodiscard]] constexpr Flag(T flags) noexcept;

        constexpr Flag(const Flag&) noexcept = default;
        constexpr Flag(Flag&&) noexcept = default;

        constexpr Flag& operator=(const Flag&) noexcept = default;
        constexpr Flag& operator=(Flag&&) noexcept = default;

        constexpr ~Flag() noexcept = default;

        [[nodiscard]] constexpr operator T() const noexcept;

        [[nodiscard]] constexpr Flag operator|(T flag) const noexcept;
        constexpr void operator|=(T flag) noexcept;

        [[nodiscard]] constexpr bool contains(T flag) const noexcept;

        [[nodiscard]] constexpr bool operator==(const T& other) const noexcept;
        [[nodiscard]] constexpr bool operator!=(const T& other) const noexcept;

        [[nodiscard]] constexpr bool operator==(const Flag&) const noexcept = default;
        [[nodiscard]] constexpr bool operator!=(const Flag&) const noexcept = default;
    private:
        T m_flags{};
    };
}

namespace ikk
{
    template<class T>
    constexpr Flag<T>::Flag(T flags) noexcept
        : m_flags(flags)
    {
    }

    template<class T>
    constexpr Flag<T>::operator T() const noexcept
    {
        return this->m_flags;
    }

    template<class T>
    constexpr Flag<T> Flag<T>::operator|(T flag) const noexcept
    {
        return Flag<T>{static_cast<T>(static_cast<Type>(this->m_flags) | static_cast<Type>(flag))};
    }

    template<class T>
    constexpr void Flag<T>::operator|=(T flag) noexcept
    {
        this->m_flags = static_cast<T>(static_cast<Type>(this->m_flags) | static_cast<Type>(flag));
    }

    template<class T>
    constexpr bool Flag<T>::contains(T flag) const noexcept
    {
        return static_cast<Type>(static_cast<Type>(this->m_flags) & static_cast<Type>(flag)) == static_cast<Type>(flag);
    }

    template<class T>
    constexpr bool Flag<T>::operator==(const T& other) const noexcept
    {
        return this->m_flags == other;
    }

    template<class T>
    constexpr bool Flag<T>::operator!=(const T& other) const noexcept
    {
        return this->m_flags != other;
    }
}