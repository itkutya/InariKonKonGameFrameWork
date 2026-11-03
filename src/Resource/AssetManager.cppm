module;

#include <unordered_map>
#include <filesystem>
#include <functional>
#include <expected>
#include <variant>
#include <utility>
#include <future>
#include <ranges>

export module AssetManager;

import ResourceLoader;
import Asset;
import Error;

export namespace ikk
{
    class AssetManager
    {
    public:
        AssetManager() noexcept = default;

        AssetManager(const AssetManager&) noexcept = default;
        AssetManager(AssetManager&&) noexcept = default;

        AssetManager& operator=(const AssetManager&) noexcept = default;
        AssetManager& operator=(AssetManager&&) noexcept = default;

        ~AssetManager() noexcept = default;

        void update() noexcept;

        template<class T, LoadingMode Mode = LoadingMode::Async, class... Args>
        [[nodiscard]] Asset<T> preload(const std::filesystem::path& path, Args&&... args) noexcept;

        template<class T>
        [[nodiscard]] const T* getResource(Asset<T> id) const noexcept;

        template<class T>
        [[nodiscard]] T* getResource(Asset<T> id) noexcept;
    private:
        struct PendingAsset
        {
            std::move_only_function<void()> update{};
        };

        std::unordered_map<Resource::ID, Resource::Types> m_assets{};
        std::unordered_map<Resource::ID, PendingAsset> m_pendingAssets{};

        Resource::ID m_nextID{0};
    };
}

namespace ikk
{
    void AssetManager::update() noexcept
    {
        for (auto& [update] : this->m_pendingAssets | std::views::values)
            update();
    }

    template<class T, LoadingMode Mode, class... Args>
    Asset<T> AssetManager::preload(const std::filesystem::path& path, Args&&... args) noexcept
    {
        using ResourceLoader = ResourceLoader<T, Mode>;

        if constexpr (Mode == LoadingMode::Immediate)
        {
            if (typename ResourceLoader::ReturnType result = ResourceLoader::load(path, std::forward<Args>(args)...); result.has_value() == true)
            {
                const Resource::ID id{++this->m_nextID};
                this->m_assets.emplace(id, std::move(result.value()));
                return Asset<T>{id, &std::get<T>(this->m_assets[id]), LoadingState::Loaded};
            }
        }
        else if constexpr (Mode == LoadingMode::Async)
        {
            typename ResourceLoader::ReturnType result = ResourceLoader::load(path, std::forward<Args>(args)...);
            const Resource::ID id{++this->m_nextID};

            this->m_pendingAssets.emplace(id, PendingAsset{
                [this, id, future = std::move(result)] mutable noexcept
                {
                    if (future.valid() == false)
                        return;

                    if (future.wait_for(std::chrono::seconds{0}) != std::future_status::ready)
                        return;

                    if (auto r = future.get(); r.has_value() == true)
                        this->m_assets.emplace(id, std::move(r.value()));
                    //TODO:
                    //this->m_pendingAssets.erase(id);
                }});

            return Asset<T>{id, nullptr, LoadingState::Loading};
        }
        return Asset<T>{};
    }

    template<class T>
    const T* AssetManager::getResource(Asset<T> id) const noexcept
    {
        const auto it = this->m_assets.find(id.id());

        if (it == this->m_assets.end())
            return nullptr;

        return std::get_if<T>(&it->second);
    }

    template<class T>
    T* AssetManager::getResource(Asset<T> id) noexcept
    {
        const auto it = this->m_assets.find(id.id());

        if (it == this->m_assets.end())
            return nullptr;

        return std::get_if<T>(&it->second);
    }
}