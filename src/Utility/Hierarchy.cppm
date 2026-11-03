module;

#include <algorithm>
#include <ranges>
#include <vector>

export module Hierarchy;

export namespace ikk
{
    template<class T>
    class Hierarchy
    {
    public:
        Hierarchy(const Hierarchy&) noexcept = delete;
        Hierarchy& operator=(const Hierarchy&) noexcept = delete;

        [[nodiscard]] const std::vector<T*>& getChildren() const noexcept;
        [[nodiscard]] const T* getParent() const noexcept;

        void setParent(T& parent) noexcept;
        void removeParent() noexcept;

        void addChild(T& child) noexcept;
        void removeChild(T& child) noexcept;
    private:
        T* m_parent = nullptr;
        std::vector<T*> m_children;

        Hierarchy() noexcept = default;

        Hierarchy(Hierarchy&& other) noexcept;
        Hierarchy& operator=(Hierarchy&& other) noexcept;

        ~Hierarchy() noexcept;

        [[nodiscard]] std::vector<T*>& getChildren() noexcept;
        [[nodiscard]] T* getParent() noexcept;

        [[nodiscard]] const T& self() const noexcept;
        [[nodiscard]] T& self() noexcept;

        friend T;
    };
}

namespace ikk
{
    template<class T>
    const std::vector<T*>& Hierarchy<T>::getChildren() const noexcept
    {
        return this->m_children;
    }

    template<class T>
    const T* Hierarchy<T>::getParent() const noexcept
    {
        return this->m_parent;
    }

    template<class T>
    void Hierarchy<T>::setParent(T& parent) noexcept
    {
        parent.addChild(this->self());
    }

    template<class T>
    void Hierarchy<T>::removeParent() noexcept
    {
        if (this->m_parent != nullptr)
            this->m_parent->removeChild(this->self());
    }

    template<class T>
    void Hierarchy<T>::addChild(T& child) noexcept
    {
        if (std::ranges::find(this->m_children, &child) != this->m_children.end())
            return;

        T& self = this->self();

        for (T* p = &self; p != nullptr; p = p->getParent())
            if (p == &child)
                return;

        if (child.getParent() != nullptr)
            child.removeParent();

        child.m_parent = &self;
        this->m_children.emplace_back(&child);
    }

    template<class T>
    void Hierarchy<T>::removeChild(T& child) noexcept
    {
        if (auto it = std::ranges::find(this->m_children, &child); it != this->m_children.end())
        {
            child.m_parent = nullptr;
            *it = this->m_children.back();
            this->m_children.pop_back();
        }
    }

    template<class T>
    Hierarchy<T>::Hierarchy(Hierarchy&& other) noexcept
    {
        if (other.m_parent != nullptr)
            other.m_parent->addChild(this->self());

        while (other.m_children.empty() == false)
            this->addChild(*other.m_children.back());
    }

    template<class T>
    Hierarchy<T>& Hierarchy<T>::operator=(Hierarchy&& other) noexcept
    {
        if (this != &other)
        {
            while (this->m_children.empty() == false)
                this->removeChild(*this->m_children.back());

            this->removeParent();

            if (other.m_parent != nullptr)
                other.m_parent->addChild(this->self());

            while (other.m_children.empty() == false)
                this->addChild(*other.m_children.back());
        }
        return *this;
    }

    template<class T>
    Hierarchy<T>::~Hierarchy() noexcept
    {
        while (this->m_children.empty() == false)
            this->m_children.back()->removeParent();

        if (this->m_parent != nullptr)
            this->removeParent();
    }

    template<class T>
    std::vector<T*>& Hierarchy<T>::getChildren() noexcept
    {
        return this->m_children;
    }

    template<class T>
    T* Hierarchy<T>::getParent() noexcept
    {
        return this->m_parent;
    }

    template<class T>
    const T& Hierarchy<T>::self() const noexcept
    {
        return *static_cast<const T*>(this);
    }

    template<class T>
    T& Hierarchy<T>::self() noexcept
    {
        return *static_cast<T*>(this);
    }
}