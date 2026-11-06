module;

#include <filesystem>
#include <functional>
#include <future>
#include <optional>
#include <utility>

export module ResourceLoader;

import NonConstructible;
import ResourceTraits;
import Asset;

export namespace ikk
{
    template<ResourceType T, LoadingMode Mode>
    struct ResourceLoader : NonConstructible
    {
        template<class... Args>
        [[nodiscard]] static Asset<T> load(const std::filesystem::path& path, Args&&... args) noexcept;
    };
}

namespace ikk
{
    template<ResourceType T, LoadingMode Mode>
    template<class... Args>
    Asset<T> ResourceLoader<T, Mode>::load(const std::filesystem::path& path, Args&&... args) noexcept
    {
        if constexpr (Mode == LoadingMode::Immediate)
        {
            try
            {
                return Asset<T>{ path, std::forward<Args>(args)... };
            }
            catch (...)
            {
                return Asset<T>{};
            }
        }
        else if constexpr (Mode == LoadingMode::Async)
        {
            return Asset<T>{std::move(std::async(std::launch::async,
                [path, ...args = std::forward<Args>(args)] mutable noexcept -> T
                {
                    std::optional<std::reference_wrapper<T>> resource = ResourceLoader<T, LoadingMode::Immediate>::load(path, std::forward<Args>(args)...).getResource();
                    if (resource.has_value() == false) //TODO: add something to this failure point
                        return T{};

                    return std::move(resource.value().get());
                }))};
        }
        else if constexpr (Mode == LoadingMode::Deferred)
        {
            return Asset<T>{std::move(std::async(std::launch::deferred,
                [path, ...args = std::forward<Args>(args)] mutable noexcept -> T
                {
                    std::optional<std::reference_wrapper<T>> resource = ResourceLoader<T, LoadingMode::Immediate>::load(path, std::forward<Args>(args)...).getResource();
                    if (resource.has_value() == false) //TODO: add something to this failure point
                        return T{};

                    return std::move(resource.value().get());
                }))};
        }
        else
        {
            static_assert(false, "Invalid loading mode.");
            return Asset<T>{};
        }
    }
}