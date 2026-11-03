export module DebugUtils;

export namespace ikk
{
    [[nodiscard]] inline consteval bool isDebugBuild() noexcept
    {
#ifndef NDEBUG
        return true;
#else
        return false;
#endif
    }

    [[nodiscard]] inline consteval bool isReleaseBuild() noexcept
    {
        return isDebugBuild() == false;
    }
}