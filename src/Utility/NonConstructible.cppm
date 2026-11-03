export module NonConstructible;

import NonCopyable;
import NonMovable;

export namespace ikk
{
    class NonConstructible : public NonCopyable, public NonMovable
    {
    public:
        constexpr NonConstructible() noexcept = delete;
    protected:
        constexpr ~NonConstructible() noexcept = default;
    };
}