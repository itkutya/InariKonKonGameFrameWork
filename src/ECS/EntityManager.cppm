module;

#include <concepts>
#include <memory>
#include <tuple>
#include <type_traits>

#include "entt/entity/registry.hpp"

export module EntityManager;

import EntityContext;
import TypeUtils;
import Entity;

export namespace ikk
{
    class EntityManager final
    {
    public:
        EntityManager() noexcept = default;

        EntityManager(const EntityManager&) noexcept = default;
        EntityManager(EntityManager&&) noexcept = default;

        EntityManager& operator=(const EntityManager&) noexcept = default;
        EntityManager& operator=(EntityManager&&) noexcept = default;

        ~EntityManager() noexcept = default;

        [[nodiscard]] EntityCreationKey getEntityCreationKey() const noexcept;
        [[nodiscard]] Entity createEntity() const noexcept;

        template<Callable Func>
        void update(Func&& func) noexcept;
    private:
        std::shared_ptr<entt::registry> m_registry = std::make_shared<entt::registry>();

        template<bool HasEntity, class... Components, Callable Func>
        void updateImpl(Func&& func) noexcept;

        template<class Component>
        decltype(auto) getComponent(const auto& view, entt::entity entity) const noexcept;
    };
}

namespace ikk
{
    EntityCreationKey EntityManager::getEntityCreationKey() const noexcept
    {
        return EntityCreationKey{ .registry = this->m_registry.get() };
    }

    Entity EntityManager::createEntity() const noexcept
    {
        return Entity{this->getEntityCreationKey()};
    }

    template<Callable Func>
    void EntityManager::update(Func&& func) noexcept
    {
        using Function = FunctionTraits<Func>;
        using FunctionArguments = Function::ArgsTuple;
        constexpr bool HasEntity = std::same_as<std::remove_cvref_t<std::tuple_element_t<0, FunctionArguments>>, Entity>;
        constexpr std::size_t ComponentCount = std::tuple_size_v<FunctionArguments> - (HasEntity == true ? 1 : 0);

        [this, &func]<std::size_t... I>(std::index_sequence<I...>) noexcept
        {
            constexpr std::size_t offset = HasEntity ? 1 : 0;
            this->updateImpl<HasEntity, std::remove_cvref_t<std::tuple_element_t<I + offset, FunctionArguments>>...>(std::forward<Func>(func));
        }(std::make_index_sequence<ComponentCount>{});
    }

    template<bool HasEntity, class... Components, Callable Func>
    void EntityManager::updateImpl(Func&& func) noexcept
    {
        for (auto view = this->m_registry->view<Components...>(); const auto entity : view)
        {
            if constexpr (HasEntity)
                func(Entity{EntityStorage{ .id = entity, .registry = this->m_registry.get() }}, this->getComponent<Components>(view, entity)...);
            else
                func(this->getComponent<Components>(view, entity)...);
        }
    }

    template<class Component>
    decltype(auto) EntityManager::getComponent(const auto& view, entt::entity entity) const noexcept
    {
        if constexpr (std::is_empty_v<Component> == true)
            return Component{};
        else
            return view.template get<Component>(entity);
    }
}