export module NonMovable;

export namespace ikk
{
    class NonMovable
    {
    public:
        constexpr NonMovable(NonMovable&&) noexcept = delete;
        constexpr NonMovable& operator=(NonMovable&&) noexcept = delete;
    protected:
        constexpr NonMovable() noexcept = default;

        constexpr NonMovable(const NonMovable&) noexcept = default;
        constexpr NonMovable& operator=(const NonMovable&) noexcept = default;

        constexpr ~NonMovable() noexcept = default;
    };
}