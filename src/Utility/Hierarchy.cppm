export module Hierarchy;

import std;

export namespace ikk
{
    template<class T>
    class Hierarchy
    {
    public:
        Hierarchy(const Hierarchy&) noexcept = delete;
        Hierarchy(Hierarchy&&) noexcept = delete;

        Hierarchy& operator=(const Hierarchy&) noexcept = delete;
        Hierarchy& operator=(Hierarchy&&) noexcept = delete;

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

        ~Hierarchy() noexcept;

        [[nodiscard]] std::vector<T*>& getChildren() noexcept;
        [[nodiscard]] T* getParent() noexcept;

        [[nodiscard]] const T& self() const noexcept;
        [[nodiscard]] T& self() noexcept;

        void transferLinksTo(T& destination) noexcept;

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

        for (T* parent = &self; parent != nullptr; parent = parent->getParent())
            if (parent == &child)
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

    template<class T>
    void Hierarchy<T>::transferLinksTo(T& destination) noexcept
    {
        T& source = this->self();

        if (&source == &destination)
            return;

        destination.removeParent();
        while (destination.getChildren().empty() == false)
            destination.removeChild(*destination.getChildren().back());

        if (this->m_parent != nullptr)
        {
            T* parent = this->m_parent;
            parent->removeChild(source);
            parent->addChild(destination);
        }

        while (this->m_children.empty() == false)
            destination.addChild(*this->m_children.back());
    }
}