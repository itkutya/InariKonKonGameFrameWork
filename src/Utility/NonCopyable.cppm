export module NonCopyable;

export namespace ikk
{
    class NonCopyable
    {
    public:
        constexpr NonCopyable(const NonCopyable&) noexcept = delete;
        constexpr NonCopyable& operator=(const NonCopyable&) noexcept = delete;
    protected:
        constexpr NonCopyable() noexcept = default;

        constexpr NonCopyable(NonCopyable&&) noexcept = default;
        constexpr NonCopyable& operator=(NonCopyable&&) noexcept = default;

        constexpr ~NonCopyable() noexcept = default;
    };
}