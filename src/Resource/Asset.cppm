module;

#include <cassert>
#include <chrono>
#include <filesystem>
#include <functional>
#include <future>
#include <optional>
#include <utility>
#include <variant>

export module Asset;

import ResourceTraits;
import Time;

export namespace ikk
{
    template<ResourceType T>
    class Asset
    {
    public:
        Asset() noexcept = default;

        template<class... Args>
        explicit Asset(const std::filesystem::path& path, Args&&... args) noexcept;

        explicit Asset(std::future<T>&& resource) noexcept;

        Asset(const Asset&) noexcept = default;
        Asset(Asset&&) noexcept = default;

        Asset& operator=(const Asset&) noexcept = default;
        Asset& operator=(Asset&&) noexcept = default;

        ~Asset() noexcept = default;

        [[nodiscard]] operator T() noexcept;

        [[nodiscard]] bool holdsResource() const noexcept;
        [[nodiscard]] bool isLoaded(const Time& waitFor = seconds(0)) noexcept;

        [[nodiscard]] LoadingState getLoadingState() const noexcept;
        [[nodiscard]] std::optional<std::reference_wrapper<T>> getResource() noexcept;

        void unload() noexcept;
    private:
        struct ImmediateResource
        {
            T resource;
        };

        struct FutureResource
        {
            std::future<T> resource{};
        };

        std::variant<std::monostate, ImmediateResource, FutureResource> m_resource{};
        LoadingState m_loadingState{ LoadingState::Unloaded };
    };
}

namespace ikk
{
    template<ResourceType T>
    template<class ... Args>
    Asset<T>::Asset(const std::filesystem::path& path, Args&&... args) noexcept
        : m_resource(ImmediateResource{ .resource = T{ path, std::forward<Args>(args)... } }),
          m_loadingState(LoadingState::Loaded)
    {
    }

    template<ResourceType T>
    Asset<T>::Asset(std::future<T>&& resource) noexcept
        : m_resource(FutureResource{ .resource = std::move(resource) }),
          m_loadingState(LoadingState::Loading)
    {
    }

    template<ResourceType T>
    Asset<T>::operator T() noexcept
    {
        assert(this->isLoaded() == true);
        return this->getResource().value();
    }

    template<ResourceType T>
    bool Asset<T>::holdsResource() const noexcept
    {
        return std::holds_alternative<std::monostate>(this->m_resource) == false;
    }

    template<ResourceType T>
    bool Asset<T>::isLoaded(const Time& waitFor) noexcept
    {
        if (std::holds_alternative<FutureResource>(this->m_resource))
        {
            try
            {
                if (auto& [resource] = std::get<FutureResource>(this->m_resource);
                resource.wait_for(waitFor.toDuration<std::chrono::nanoseconds>()) == std::future_status::ready)
                {
                    this->m_loadingState = LoadingState::Loaded;
                    return true;
                }
                return false;
            }
            catch (...)
            {
                this->m_loadingState = LoadingState::Failed;
                return false;
            }
        }
        else
            return this->m_loadingState == LoadingState::Loaded;
    }

    template<ResourceType T>
    LoadingState Asset<T>::getLoadingState() const noexcept
    {
        return this->m_loadingState;
    }

    template<ResourceType T>
    std::optional<std::reference_wrapper<T>> Asset<T>::getResource() noexcept
    {
        if (this->isLoaded() == false)
            return std::nullopt;

        if (std::holds_alternative<ImmediateResource>(this->m_resource))
            return std::ref(std::get<ImmediateResource>(this->m_resource).resource);

        try
        {
            this->m_resource = ImmediateResource{ .resource = std::move(std::get<FutureResource>(this->m_resource).resource.get()) };
            return std::ref(std::get<ImmediateResource>(this->m_resource).resource);
        }
        catch (...)
        {
            this->m_loadingState = LoadingState::Failed;
            return std::nullopt;
        }
    }

    template<ResourceType T>
    void Asset<T>::unload() noexcept
    {
        if (this->isLoaded() == true)
        {
            try
            {
                this->m_resource = std::monostate{};
                this->m_loadingState = LoadingState::Unloaded;
            }
            catch (...)
            {
                this->m_loadingState = LoadingState::Failed;
            }
        }

        //TODO: unload failed resources...
    }
}