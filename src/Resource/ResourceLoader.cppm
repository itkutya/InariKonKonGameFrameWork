module;

#include <concepts>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <future>
#include <type_traits>
#include <utility>
#include <variant>

export module ResourceLoader;

import NonConstructible;
import Error;

import Shader;

export namespace ikk
{
    enum struct LoadingMode
    {
        Immediate,
        Async,
        OnDemand,
        Deferred = OnDemand
    };

    enum struct LoadingState
    {
        Unloaded,
        Loading,
        Loaded,
        Failed
    };

    template<class T, LoadingMode Mode>
    struct ResourceLoader : NonConstructible
    {
        using ResourceType = T;
        using ResourceResult = std::expected<ResourceType, Error>;
        using ReturnType = std::conditional_t<Mode != LoadingMode::Immediate, std::future<ResourceResult>, ResourceResult>;

        template<class... Args>
        [[nodiscard]] static ReturnType load(const std::filesystem::path& path, Args&&... args) noexcept;
    };

    template<class T, class... Args>
    concept ImmediateResource = requires(const std::filesystem::path& path, Args&&... args)
    {
        ResourceLoader<T, LoadingMode::Immediate>::load(path, std::forward<Args>(args)...);
    };

    template<class T, class... Args>
    concept AsyncResource = requires(const std::filesystem::path& path, Args&&... args)
    {
        ResourceLoader<T, LoadingMode::Async>::load(path, std::forward<Args>(args)...);
    };

    template<class T, class... Args>
    concept DeferredResource = requires(const std::filesystem::path& path, Args&&... args)
    {
        ResourceLoader<T, LoadingMode::Deferred>::load(path, std::forward<Args>(args)...);
    };

    template<class T, class... Args>
    concept AnyResource = ImmediateResource<T, Args...> || AsyncResource<T, Args...> || DeferredResource<T, Args...>;

    struct Resource
    {
        using ID = std::uint32_t;
        using Types = std::variant<Shader>;
    };
}

namespace ikk
{
    template<class T, LoadingMode Mode>
    struct ResourceLoaderImpl;

    template<class T, LoadingMode Mode>
    template<class... Args>
    ResourceLoader<T, Mode>::ReturnType ResourceLoader<T, Mode>::load(const std::filesystem::path& path, Args&&... args) noexcept
    {
        return ResourceLoaderImpl<T, Mode>::load(path, std::forward<Args>(args)...);
    }

    //TODO:
    // Create a macro or something...

    template<LoadingMode Mode>
    struct ResourceLoaderImpl<Shader, Mode>
    {
        using ResourceType = Shader;
        using ReturnType = ResourceLoader<ResourceType, Mode>::ReturnType;

        [[nodiscard]] static ReturnType load(const std::filesystem::path& path, Shader::Type type) noexcept;
    };

    template<LoadingMode Mode>
    ResourceLoaderImpl<Shader, Mode>::ReturnType ResourceLoaderImpl<Shader, Mode>::load(const std::filesystem::path& path, Shader::Type type) noexcept
    {
        if constexpr (Mode == LoadingMode::Immediate)
        {
            ResourceType resource{ type, path };
            if (resource.getSource().empty())
                return std::unexpected{Error{"Failed to load shader"}};
            return ReturnType{std::move(resource)};
        }
        else if constexpr (Mode == LoadingMode::Async)
        {
            return ReturnType{std::async(std::launch::async,
                [=] noexcept -> ResourceLoader<ResourceType, Mode>::ResourceResult
                {
                    ResourceType resource{ type, path };
                    if (resource.getSource().empty())
                        return std::unexpected{Error{"Failed to load shader"}};
                    return typename ResourceLoader<ResourceType, Mode>::ResourceResult{std::move(resource)};
                })};
        }
        else if constexpr (Mode == LoadingMode::Deferred)
        {
            return ReturnType{std::async(std::launch::deferred,
                [=] noexcept -> ResourceLoader<ResourceType, Mode>::ResourceResult
                {
                    ResourceType resource{ type, path };
                    if (resource.getSource().empty())
                        return std::unexpected{Error{"Failed to load shader"}};
                    return typename ResourceLoader<ResourceType, Mode>::ResourceResult{std::move(resource)};
                })};
        }
        else
        {
            static_assert(false, "Invalid loading mode.");
            return std::unexpected{Error{"Loading mode not implemented"}};
        }
    }

    //TODO: Add other resources...
}