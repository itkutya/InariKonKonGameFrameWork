module;

#include "entt/entity/registry.hpp"

export module EntityContext;

import std;

export namespace ikk
{
    struct EntityStorage
    {
        entt::entity id{entt::null};
        std::shared_ptr<entt::registry> registry{nullptr};
    };

    struct EntityCreationKey
    {
        std::shared_ptr<entt::registry> registry{nullptr};
    };
}