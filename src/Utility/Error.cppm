module;

#include <string_view>

export module Error;

export namespace ikk
{
    class Error final
    {
    public:
        explicit constexpr Error(std::string_view message) noexcept;

        constexpr Error(const Error&) noexcept = default;
        constexpr Error(Error&&) noexcept = default;

        constexpr Error& operator=(const Error&) noexcept = default;
        constexpr Error& operator=(Error&&) noexcept = default;

        constexpr ~Error() noexcept = default;

        [[nodiscard]] constexpr const std::string_view& what() const noexcept;
    private:
        std::string_view m_message{};
    };
}

namespace ikk
{
    constexpr Error::Error(std::string_view message) noexcept
        : m_message(message)
    {
    }

    constexpr const std::string_view& Error::what() const noexcept
    {
        return this->m_message;
    }
}