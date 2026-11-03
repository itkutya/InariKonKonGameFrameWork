module;

#include <compare>
#include <cstdint>
#include <variant>
#include <cassert>

export module Asset;

import ResourceLoader;

export namespace ikk
{
    template<AnyResource T>
    class Asset
    {
    public:
        inline static constexpr Resource::ID InvalidID = 0;

        Asset() noexcept = default;
        Asset(Resource::ID id, T* resource, LoadingState state) noexcept;

        Asset(const Asset&) noexcept = default;
        Asset(Asset&&) noexcept = default;

        Asset& operator=(const Asset&) noexcept = default;
        Asset& operator=(Asset&&) noexcept = default;

        ~Asset() noexcept = default;

        [[nodiscard]] const T* get() const noexcept;
        [[nodiscard]] T* get() noexcept;

        [[nodiscard]] const T& operator*() const noexcept;
        [[nodiscard]] T& operator*() noexcept;

        [[nodiscard]] const T* operator->() const noexcept;
        [[nodiscard]] T* operator->() noexcept;

        [[nodiscard]] LoadingState getLoadingState() const noexcept;

        [[nodiscard]] explicit operator bool() const noexcept;

        [[nodiscard]] Resource::ID id() const noexcept;

        [[nodiscard]] bool isValid() const noexcept;

        [[nodiscard]] auto operator<=>(const Asset&) const noexcept = default;
    private:
        Resource::ID m_id = InvalidID;
        T* m_resource = nullptr;
        LoadingState m_loadingState = LoadingState::Unloaded;
    };
}

namespace ikk
{
    template<AnyResource T>
    Asset<T>::Asset(Resource::ID id, T* resource, LoadingState state) noexcept
        : m_id(id), m_resource(resource), m_loadingState(state)
    {
    }

    //TODO: how much time we can wait as a parameter
    template<AnyResource T>
    const T* Asset<T>::get() const noexcept
    {
        //TODO:
        // wait till resource is loaded
        return this->m_resource;
    }

    template<AnyResource T>
    T* Asset<T>::get() noexcept
    {
        return this->m_resource;
    }

    template<AnyResource T>
    const T& Asset<T>::operator*() const noexcept
    {
        assert(this->isValid() == true);
        assert(this->m_resource != nullptr);
        return *this->m_resource;
    }

    template<AnyResource T>
    T& Asset<T>::operator*() noexcept
    {
        assert(this->isValid() == true);
        assert(this->m_resource != nullptr);
        return *this->m_resource;
    }

    template<AnyResource T>
    const T* Asset<T>::operator->() const noexcept
    {
        return this->m_resource;
    }

    template<AnyResource T>
    T* Asset<T>::operator->() noexcept
    {
        return this->m_resource;
    }

    template<AnyResource T>
    LoadingState Asset<T>::getLoadingState() const noexcept
    {
        return this->m_loadingState;
    }

    template<AnyResource T>
    Asset<T>::operator bool() const noexcept
    {
        return this->isValid() == true;
    }

    template<AnyResource T>
    bool Asset<T>::isValid() const noexcept
    {
        return this->m_id != InvalidID;
    }

    template<AnyResource T>
    Resource::ID Asset<T>::id() const noexcept
    {
        return this->m_id;
    }
}