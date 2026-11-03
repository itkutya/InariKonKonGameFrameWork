module;

#include <cstdint>
#include <type_traits>

export module Rect;

import Quaternion;
import Clamped;
import Number;
import Vector;

export namespace ikk
{
    template<Number T>
    class [[nodiscard]] Rect final
    {
    public:
        [[nodiscard]] constexpr Rect() noexcept = default;
        [[nodiscard]] constexpr Rect(T left, T top, T width, T height) noexcept;
        [[nodiscard]] constexpr Rect(Vec2<T> position, Vec2<T> size) noexcept;
        [[nodiscard]] explicit constexpr Rect(Vec4<T> vec) noexcept;

        constexpr Rect(const Rect&) noexcept = default;
        constexpr Rect(Rect&&) noexcept = default;

        constexpr Rect& operator=(const Rect&) noexcept = default;
        constexpr Rect& operator=(Rect&&) noexcept = default;

        constexpr ~Rect() noexcept = default;

        [[nodiscard]] constexpr const T& getLeft() const noexcept;
        [[nodiscard]] constexpr const T& getTop() const noexcept;
        [[nodiscard]] constexpr const T& getWidth() const noexcept;
        [[nodiscard]] constexpr const T& getHeight() const noexcept;

        [[nodiscard]] constexpr T& getLeft() noexcept;
        [[nodiscard]] constexpr T& getTop() noexcept;
        [[nodiscard]] constexpr T& getWidth() noexcept;
        [[nodiscard]] constexpr T& getHeight() noexcept;

        [[nodiscard]] constexpr T getRight() const noexcept;
        [[nodiscard]] constexpr T getBottom() const noexcept;

        template<Number U>
        [[nodiscard]] constexpr bool contains(Vec2<U> point) const noexcept;
        template<Number U>
        [[nodiscard]] constexpr bool contains(Vec2<U> point, Quaternionf rotation, Vec2<Clamped<float, 0.f, 1.f>> anchor = { 0.f, 0.f }) const noexcept;
    private:
        T m_left      = T{0};
        T m_top       = T{0};
        T m_width     = T{0};
        T m_height    = T{0};
    };

    using Rectf = Rect<float>;
    using Rectd = Rect<double>;
    using Recti = Rect<std::int32_t>;
    using RectI = Rect<std::int64_t>;
    using Rectu = Rect<std::uint32_t>;
    using RectU = Rect<std::uint64_t>;
}

namespace ikk
{
    template<Number T>
    constexpr Rect<T>::Rect(T left, T top, T width, T height) noexcept
        : m_left(left), m_top(top), m_width(width), m_height(height)
    {
    }

    template<Number T>
    constexpr Rect<T>::Rect(Vec2<T> position, Vec2<T> size) noexcept
        : Rect(position.x(), position.y(), size.x(), size.y())
    {
    }

    template<Number T>
    constexpr Rect<T>::Rect(Vec4<T> vec) noexcept
        : Rect(vec.x(), vec.y(), vec.z(), vec.w())
    {
    }

    template<Number T>
    constexpr const T& Rect<T>::getLeft() const noexcept
    {
        return this->m_left;
    }

    template<Number T>
    constexpr const T& Rect<T>::getTop() const noexcept
    {
        return this->m_top;
    }

    template<Number T>
    constexpr const T& Rect<T>::getWidth() const noexcept
    {
        return this->m_width;
    }

    template<Number T>
    constexpr const T& Rect<T>::getHeight() const noexcept
    {
        return this->m_height;
    }

    template<Number T>
    constexpr T& Rect<T>::getLeft() noexcept
    {
        return this->m_left;
    }

    template<Number T>
    constexpr T& Rect<T>::getTop() noexcept
    {
        return this->m_top;
    }

    template<Number T>
    constexpr T& Rect<T>::getWidth() noexcept
    {
        return this->m_width;
    }

    template<Number T>
    constexpr T& Rect<T>::getHeight() noexcept
    {
        return this->m_height;
    }

    template<Number T>
    constexpr T Rect<T>::getRight() const noexcept
    {
        return this->m_left + this->m_width;
    }

    template<Number T>
    constexpr T Rect<T>::getBottom() const noexcept
    {
        return this->m_top + this->m_height;
    }

    template<Number T>
    template<Number U>
    constexpr bool Rect<T>::contains(Vec2<U> point) const noexcept
    {
        return  point.x() >= this->getLeft()    &&
                point.y() >= this->getTop()     &&
                point.x() <= this->getRight()   &&
                point.y() <= this->getBottom();
    }

    //TODO:
    //Rect maybe should already contain an anchor and a rotation (?)
    template<Number T>
    template<Number U>
    constexpr bool Rect<T>::contains(Vec2<U> point, Quaternionf rotation, Vec2<Clamped<float, 0.f, 1.f>> anchor) const noexcept
    {
        using CommonType = std::common_type_t<T, U, float>;

        const auto width   = static_cast<CommonType>(this->m_width);
        const auto height  = static_cast<CommonType>(this->m_height);
        const auto left    = static_cast<CommonType>(this->m_left);
        const auto top     = static_cast<CommonType>(this->m_top);

        const auto anchorX = static_cast<CommonType>(anchor.x());
        const auto anchorY = static_cast<CommonType>(anchor.y());

        const Vec2<CommonType> pivot =
            {
                left + width * anchorX,
                top  + height * anchorY
            };

        const Vec2<CommonType> d = point - pivot;
        const Vec2<CommonType> localPoint = rotation.conjugated() * d;

        const CommonType minX = -width  * anchorX;
        const CommonType maxX =  width  * (CommonType{1} - anchorX);
        const CommonType minY = -height * anchorY;
        const CommonType maxY =  height * (CommonType{1} - anchorY);

        return minX <= localPoint.x() && localPoint.x() <= maxX &&
               minY <= localPoint.y() && localPoint.y() <= maxY;
    }
}