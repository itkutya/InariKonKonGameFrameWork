module;

#include <cstdint>
#include <functional>
#include <optional>
#include <utility>

#include "entt/entity/registry.hpp"

export module Entity;

import EntityContext;
import Log;

export namespace ikk
{
    class Entity final
    {
    public:
        Entity() noexcept = default;
        explicit Entity(EntityCreationKey key) noexcept;
        explicit Entity(EntityStorage storage) noexcept;

        Entity(const Entity& other) noexcept;
        Entity(Entity&& other) noexcept;

        Entity& operator=(const Entity& other) noexcept;
        Entity& operator=(Entity&& other) noexcept;

        ~Entity() noexcept;

        [[nodiscard]] operator bool() const noexcept;

        [[nodiscard]] bool operator==(const Entity& other) const noexcept;
        [[nodiscard]] bool operator!=(const Entity& other) const noexcept;

        [[nodiscard]] bool isValid() const noexcept;

        void create() noexcept;
        void destroy() noexcept;

        template<class T, class... Args>
        void addComponent(Args&&... args) noexcept;

        template<class T>
        [[nodiscard]] std::optional<std::reference_wrapper<const T>> getComponent() const noexcept;
        template<class T>
        [[nodiscard]] std::optional<std::reference_wrapper<T>> getComponent() noexcept;

        template<class... Ts>
        [[nodiscard]] bool hasComponents() const noexcept;

        template<class T>
        void removeComponent() noexcept;
    private:
        enum struct Type : std::uint8_t
        {
            None, Owner, Observer
        };

        Type m_type{Type::None};
        EntityStorage m_storage{};
    };
}

namespace ikk
{
    Entity::Entity(EntityCreationKey key) noexcept
        : Entity(EntityStorage{ .id = entt::null, .registry = key.registry })
    {
    }

    Entity::Entity(EntityStorage storage) noexcept
        : m_storage(storage)
    {
        if (this->m_storage.registry == nullptr)
            return;

        if (this->m_storage.registry->valid(this->m_storage.id) == true)
            this->m_type = Type::Observer;
        else
        {
            this->m_storage.id = this->m_storage.registry->create();
            this->m_type = Type::Owner;
        }
    }

    Entity::Entity(const Entity& other) noexcept
        : m_type(Type::Observer), m_storage(other.m_storage)
    {
    }

    Entity::Entity(Entity&& other) noexcept
        : m_type(std::exchange(other.m_type, Type::None)),
          m_storage(std::exchange(other.m_storage, EntityStorage{ .id = entt::null, .registry = nullptr }))
    {
    }

    Entity& Entity::operator=(const Entity& other) noexcept
    {
        if (this != &other)
        {
            this->destroy();

            this->m_type = Type::Observer;
            this->m_storage = other.m_storage;
        }
        return *this;
    }

    Entity& Entity::operator=(Entity&& other) noexcept
    {
        if (this != &other)
        {
            this->destroy();

            this->m_type = std::exchange(other.m_type, Type::None);
            this->m_storage = std::exchange(other.m_storage, EntityStorage{ .id = entt::null, .registry = nullptr });
        }
        return *this;
    }

    Entity::~Entity() noexcept
    {
        this->destroy();
    }

    Entity::operator bool() const noexcept
    {
        return this->isValid();
    }

    bool Entity::operator==(const Entity& other) const noexcept
    {
        return this->m_storage.registry == other.m_storage.registry && this->m_storage.id == other.m_storage.id;
    }

    bool Entity::operator!=(const Entity& other) const noexcept
    {
        return !(*this == other);
    }

    bool Entity::isValid() const noexcept
    {
        return this->m_storage.registry != nullptr && this->m_storage.registry->valid(this->m_storage.id) == true;
    }

    void Entity::create() noexcept
    {
        if (this->m_type == Type::Observer)
        {
            Log::Error("Cannot create an entity through an observer entity.");
            return;
        }

        if (this->m_type == Type::Owner)
        {
            Log::Error("Cannot create an entity twice. Call destroy() first.");
            return;
        }

        if (this->m_storage.registry != nullptr)
        {
            this->m_storage.id = this->m_storage.registry->create();
            this->m_type = Type::Owner;
        }
    }

    void Entity::destroy() noexcept
    {
        if (this->m_type == Type::Owner && this->isValid() == true)
        {
            this->m_storage.registry->destroy(this->m_storage.id);
            this->m_storage.id = entt::null;
            this->m_type = Type::None;
        }
    }

    template<class T, class... Args>
    void Entity::addComponent(Args&&... args) noexcept
    {
        if (this->m_type != Type::Owner)
        {
            Log::Error("Cannot modify an entity through an observer entity.");
            return;
        }

        if (this->hasComponents<T>() == false)
            this->m_storage.registry->emplace<T>(this->m_storage.id, std::forward<Args>(args)...);
    }

    template<class T>
    std::optional<std::reference_wrapper<const T>> Entity::getComponent() const noexcept
    {
        if (this->hasComponents<T>() == true)
            return std::optional<std::reference_wrapper<const T>>{this->m_storage.registry->get<T>(this->m_storage.id)};
        return std::nullopt;
    }

    template<class T>
    std::optional<std::reference_wrapper<T>> Entity::getComponent() noexcept
    {
        if (this->m_type != Type::Owner)
        {
            Log::Error("Cannot modify an entity through an observer entity.");
            return std::nullopt;
        }

        if (this->hasComponents<T>() == true)
            return std::optional<std::reference_wrapper<T>>{this->m_storage.registry->get<T>(this->m_storage.id)};
        return std::nullopt;
    }

    template<class... Ts>
    bool Entity::hasComponents() const noexcept
    {
        return this->isValid() == true && this->m_storage.registry->all_of<Ts...>(this->m_storage.id) == true;
    }

    template<class T>
    void Entity::removeComponent() noexcept
    {
        if (this->m_type != Type::Owner)
        {
            Log::Error("Cannot modify an entity through an observer entity.");
            return;
        }

        if (this->hasComponents<T>() == true)
            this->m_storage.registry->remove<T>(this->m_storage.id);
    }
}