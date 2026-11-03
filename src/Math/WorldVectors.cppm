export module WorldVectors;

import NonConstructible;
import Dimension;
import Vector;

export namespace ikk
{
    template<Dimension D>
    struct World;

    template<>
    struct World<Dimension::_2D> final : NonConstructible
    {
        inline static constexpr Vec2f getRight() noexcept     { return  Vec2f::X(); }
        inline static constexpr Vec2f getUp() noexcept        { return  Vec2f::Y(); }

        inline static constexpr Vec2f getLeft() noexcept      { return -Vec2f::X(); }
        inline static constexpr Vec2f getDown() noexcept      { return -Vec2f::Y(); }
    };

    using World2D = World<Dimension::_2D>;

    template<>
    struct World<Dimension::_3D> final : NonConstructible
    {
        inline static constexpr Vec3f getRight() noexcept     { return  Vec3f::X(); }
        inline static constexpr Vec3f getUp() noexcept        { return  Vec3f::Y(); }
        inline static constexpr Vec3f getForward() noexcept   { return  Vec3f::Z(); }

        inline static constexpr Vec3f getLeft() noexcept      { return -Vec3f::X(); }
        inline static constexpr Vec3f getDown() noexcept      { return -Vec3f::Y(); }
        inline static constexpr Vec3f getBackward() noexcept  { return -Vec3f::Z(); }
    };

    using World3D = World<Dimension::_3D>;
}