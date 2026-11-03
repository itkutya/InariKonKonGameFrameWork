module;

#include "entt/entity/registry.hpp"

export module EntityContext;

export namespace ikk
{
    struct EntityStorage
    {
        entt::entity id{entt::null};
        entt::registry* registry{nullptr};
    };

    struct EntityCreationKey
    {
        entt::registry* registry{nullptr};
    };
}